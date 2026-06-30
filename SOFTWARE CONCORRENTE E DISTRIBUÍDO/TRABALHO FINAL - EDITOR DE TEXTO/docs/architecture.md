# Arquitetura — Editor de Documentos Compartilhado

## 1. Propósito

Um editor de documentos colaborativo em que muitos clientes remotos visualizam e
editam os mesmos documentos ao mesmo tempo. As edições são transmitidas ao vivo a
cada participante, e jobs de segundo plano no servidor (corretor ortográfico,
formatador) anotam o texto concorrentemente. O sistema foi construído para
exercitar todo o conjunto de preocupações de sistemas concorrentes e
distribuídos: controle de concorrência, particionamento, replicação,
consistência, disponibilidade, e interação síncrona e assíncrona, com componentes
em mais de uma linguagem.

## 2. Visão geral dos componentes

```
        clientes na Internet  (UI web + clientes simulados)
              │   WebSocket (assíncrono)     REST (síncrono)
              ▼
   ┌─────────────────────────────────────────────┐
   │  GATEWAY / BORDA  (Node.js)                   │
   │  terminador REST + WebSocket, sessões,        │
   │  ROTEADOR de shards, RPC síncrono aos primários,│
   │  difusão Redis pub/sub → WebSocket            │
   └───────┬───────────────────────────┬──────────┘
     RPC síncrono                  Redis pub/sub
           │                            ▲
           ▼                            │
   ┌──────────────────────────────┐    │
   │  SERVIÇO DE DOCUMENTOS (Python)│   │
   │  sharda: primário ⇄ réplica   │    │
   │  shardb: primário ⇄ réplica   │    │
   │  sequenciador + log + lease   │    │
   └───────┬──────────────────────┘    │
   Redis Streams (jobs)                 │ publica anotações
           ▼                            │
   ┌──────────────────────────────┐    │
   │  WORKERS (Python)             │────┘
   │  pool de corretor, formatador │
   └──────────────────────────────┘
   ┌──────────────────────────────┐
   │  REDIS — pub/sub, streams,    │
   │  mapa de shards, leases       │
   └──────────────────────────────┘
```

| Componente | Linguagem | Papel |
|------------|-----------|-------|
| Gateway / Borda | Node.js | Endpoints públicos REST + WebSocket; valida e roteia; resolve o primário do shard; difunde os eventos do Redis para os clientes WebSocket. |
| Serviço de documentos | Python (FastAPI) | Estado autoritativo por shard; **sequenciador** que atribui um `seq` global por doc; log de operações; replicação; eleição de primário por lease. |
| Workers de segundo plano | Python | Pools de corretor ortográfico e formatador consumindo Redis Streams concorrentemente com a edição. |
| Redis | — | Pub/Sub (notificações), Streams (fila de jobs + transporte de replicação), chaves para o mapa de shards e leases. |
| UI web / clientes simulados | JS / Python / Node | Editor visível a humanos + clientes programados que conduzem a demo. |

## 3. Paradigmas de interação

* **Cliente–servidor (requisição/resposta):** navegadores/clientes ↔ gateway
  (REST + WS); gateway ↔ serviço de documentos (RPC HTTP/JSON).
* **Publicação–assinatura:** os primários publicam `op.applied`; os workers
  publicam anotações; o gateway assina e envia aos clientes WebSocket.
* **Mensageria / fila:** Redis Streams transportam jobs de segundo plano (grupos
  de consumidores) e o log de operações de replicação.

## 4. Modelo de dados

Um documento é um snapshot de texto mais um log ordenado de operações:

```
texto = fold(snapshotText, opLog ordenado por seq)
```

* **Operação de edição (cliente → servidor):**
  `{ docId, clientId, baseVersion, op:{kind:"insert"|"delete", pos, text|len}, opId }`
* **Entrada de log (após sequenciar):** o acima mais `seq`, `op` transformada,
  `ts`, `appliedBy`.
* **Snapshot:** `{ docId, baseVersion, text }`, atualizado a cada
  `SNAPSHOT_EVERY` operações e guardado no Redis (`snap:{docId}`).

## 5. Controle de concorrência — sequenciador central + log de operações

O **primário** do shard é o único escritor autoritativo de cada documento e
serializa as operações com uma trava por documento, produzindo uma ordem total
(`seq`). Quando o `baseVersion` de um cliente está atrasado, o primário
**rebaseia** de forma determinística a posição da operação sobre as operações
sequenciadas no intervalo, então atribui o próximo `seq` e difunde a operação
transformada. Como cada réplica/cliente aplica as mesmas operações transformadas
na ordem de `seq`, todas as cópias convergem. Veja `doc_service/ops.py` e
`tests/test_ops.py`.

## 6. Particionamento

Os documentos são particionados por `docId` sobre um anel de hash consistente
(`gateway/src/router.js`). O mapa de shards vive no Redis (`shardmap`). O gateway
faz hash do `docId` → shard e então resolve o primário atual do shard pela chave
de lease. Os workers são divididos em pools independentes de corretor e
formatador — particionamento **funcional**, além do particionamento de dados.

## 7. Replicação

Cada shard tem um primário e uma ou mais réplicas. O primário acrescenta toda
entrada de log a um Redis Stream `replog:shard:{id}`; as réplicas o consomem na
ordem de `seq` e aplicam as entradas de forma idempotente (entradas em/abaixo da
cabeça local são ignoradas). Leituras podem ser servidas por uma réplica
(`?replica=1`), que retorna o seu próprio `seq` possivelmente defasado.
`REPL_MODE=sync` faz o primário esperar o ack de uma réplica antes de confirmar
uma escrita.

## 8. Disponibilidade — failover baseado em lease

O status de primário é um lease no Redis (`lease:shard:{id}`, `SET NX PX`)
renovado periodicamente. Se o primário cai, o lease expira e uma réplica o
adquire, drena o stream de replicação e anuncia `primary.changed` em
`cluster.events`. O gateway re-resolve o primário e **tenta novamente** as
escritas em andamento, então a edição continua durante o failover, com `seq`
contínuo e sem operações perdidas (o replay idempotente garante a segurança).
Veja `doc_service/lease.py` e `doc_service/replication.py`.

## 9. Síncrono vs assíncrono

* **Síncrono (bloqueante):** REST `POST /docs`, `GET /docs/{id}`,
  `GET /docs/{id}/ops`, `POST /docs/{id}/snapshot|save`, e o RPC gateway→primário.
* **Assíncrono:** streaming de ops por WebSocket, difusão via Redis Pub/Sub de
  `op.applied` e anotações, e despacho de jobs via Redis Streams.

## 10. Processamento de segundo plano

O primário marca os documentos editados como sujos; um flusher com debounce
enfileira jobs de corretor e de formatação no Redis Streams. Os pools de workers
consomem via grupos de consumidores (concorrente, ao-menos-uma-vez, com
recuperação por `XAUTOCLAIM`) e publicam anotações em
`doc:{docId}:annotations`, que o gateway repassa aos clientes — tudo isso
enquanto a edição continua.

## 11. Implantação

Um único host EC2 roda cada componente como um container via `docker-compose`
(1 gateway, 2 shards × {primário, réplica}, 2 workers de corretor, 1 formatador,
Redis). Endpoints públicos: `:8080` (REST + UI) e `:8081` (WebSocket). Como todo
o estado de coordenação vive no Redis, os componentes podem mais tarde ser
espalhados por várias instâncias EC2 apontando para um Redis compartilhado. Veja
`docs/implementation.md` e o README para os passos exatos.
