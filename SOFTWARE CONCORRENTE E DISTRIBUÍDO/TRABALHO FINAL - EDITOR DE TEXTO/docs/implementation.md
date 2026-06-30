# Notas de Implementação

Este documento mapeia o design ao código real e explica os algoritmos-chave e o
protocolo de comunicação.

## Mapa do código-fonte

```
gateway/        borda em Node.js
  src/server.js     API REST, bootstrap do servidor WS, fiação do Redis, seed do shardmap
  src/router.js     anel de hash consistente; docId→shard; resolução de primário/réplica
  src/rpcClient.js  RPC HTTP/JSON ao doc-service; re-resolução do primário + retry de escrita
  src/redisBus.js   assinante pub/sub → despacho (eventos de doc + de cluster)
  src/wsHub.js      salas WebSocket por doc; tratamento das mensagens do cliente; difusão
  src/names.js      nomes de chaves/canais/streams do Redis (espelha doc_service/names.py)

doc_service/    nó de shard em Python (FastAPI); a mesma imagem roda primário ou réplica
  app.py            endpoints RPC; pipeline de escrita; orquestração da troca de papel
  ops.py            apply/rebase/fold puros (testados em unidade)
  store.py          DocState + DocStore; log de ops, snapshots, idempotência, trava por doc
  replication.py    produtor (primário) / consumidor (réplica) do log via Redis Stream
  lease.py          eleição de primário por lease no Redis + heartbeat + callback de failover
  jobs.py           enfileiramento com debounce de jobs de corretor/formatação (Redis Streams)
  config.py         configuração por env
  names.py          nomes de chaves/canais/streams do Redis
  tests/            testes de ops + replicação (pytest ou tests/run_ops.py)

workers/        processadores de segundo plano em Python
  worker_common.py     loop XREADGROUP de grupo de consumidores + recuperação XAUTOCLAIM
  spellcheck_worker.py  jobs:spellcheck → anotações por faixa
  formatter_worker.py   jobs:format → sugestões de formatação
  dictionary.py         conjunto de palavras (pt-BR) + sugestões a distância de edição 1

webui/          editor mínimo no navegador (textarea, cliente WS, painel de anotações)
clients/        sim_client.py (asyncio) + sim_client.js (Node) clientes simulados
scripts/        demo.sh, kill_primary.sh, wait_for_health.sh
test-data/      dicionário words.txt (pt-BR), documentos de exemplo, trace de edições concorrentes
```

## Pipeline de escrita (primário) — `app.py:rpc_op`

1. `_require_primary()` — não-primários retornam **409** para o gateway re-resolver.
2. Adquire o `asyncio.Lock` por documento (serializa este doc; outros docs rodam
   concorrentemente).
3. **Idempotência:** se o `opId` já foi aplicado, retorna o resultado anterior.
4. **Rebase:** `rebase(op, intervening_ops_since(baseVersion))`.
5. Atribui `seq = doc.seq + 1`; monta a entrada de log; `append_applied` (aplica
   o texto, avança o `seq`, pode compactar em um snapshot).
6. **Replica:** `XADD replog:shard:{id}` (durabilidade antes do ack).
   `REPL_MODE=sync` espera o ack de uma réplica.
7. Marca o doc como sujo para os jobs de segundo plano.
8. **Publica** `op.applied` em `doc:{docId}` (o caminho de difusão assíncrono).
9. Retorna `{seq, transformedOp}` ao gateway (o ack síncrono).

## Algoritmo de rebase — `ops.py`

Para cada operação `e` já sequenciada e aplicada após o `baseVersion` do cliente,
desloca a posição da op recebida:
* `e` é uma inserção em `e.pos ≤ pos` → `pos += len(e.text)` (empates empurram a
  op recebida para a direita, de forma determinística).
* `e` é uma remoção começando antes de `pos` → `pos -= min(e.len, pos − e.pos)`.

A op transformada é a que é sequenciada e difundida, então réplicas e clientes
que dobram as mesmas ops transformadas e ordenadas chegam ao mesmo texto. Provado
em `tests/test_ops.py::test_concurrent_inserts_converge` e
`tests/test_replication.py::test_replica_catches_up_to_primary`.

## Papéis, leases e failover

* O papel é **dinâmico**: um nó é primário se, e somente se, detém
  `lease:shard:{id}`.
* `PREFERRED_ROLE=primary` apenas faz esse nó tentar adquirir imediatamente; as
  réplicas esperam `REPLICA_START_DELAY` para que o primário pretendido vença no boot.
* No `on_role_change(primary=True)`: `drain()` da cauda do replog, para de
  consumir, inicia o flusher de jobs e anuncia `primary.changed`.
* No gateway, uma escrita que recebe 409 ou erro de rede descarta o cache do
  primário, faz backoff e tenta de novo (`rpcClient.write`, orçamento ~8s) —
  maior que uma tomada de lease no pior caso, então as escritas sobrevivem à
  transição.

## Protocolo de comunicação

**WebSocket (cliente ↔ gateway).** Entrada: `join`, `op`, `cursor`, `ping`.
Saída: `resync` (texto + seq completos no join/buraco), `op.applied`,
`annotation`, `presence`, `pong`, `error`.

**REST (cliente ↔ gateway).** `POST /docs`, `GET /docs/{id}[?replica=1]`,
`GET /docs/{id}/ops?since=N`, `POST /docs/{id}/snapshot`, `POST /docs/{id}/save`,
`GET /health`, `GET /shardmap`, `GET /cluster`.

**RPC (gateway → doc-service).** `POST /rpc/create`, `POST /rpc/op`,
`POST /rpc/snapshot`, `GET /rpc/doc/{id}`, `GET /rpc/ops/{id}?since=N`,
`GET /role`, `GET /health`.

**Redis.** Canais `doc:{id}`, `doc:{id}:annotations`, `doc:{id}:presence`,
`cluster.events`. Streams `replog:shard:{id}`, `jobs:spellcheck`, `jobs:format`
(grupos `spellers`, `formatters`). Chaves `shardmap`, `lease:shard:{id}`,
`snap:{id}`.

## Verificação ortográfica em português

O corretor reconhece português do Brasil:
* o tokenizer em `workers/spellcheck_worker.py` usa `[A-Za-zÀ-ÿ']+`, aceitando
  letras acentuadas (ç, ã, é, í, ó, ú...);
* o dicionário (`test-data/words.txt`) é uma lista curada de palavras em
  português; `workers/dictionary.py` gera sugestões a distância de edição 1 com
  um alfabeto que inclui letras acentuadas;
* o formatador (`workers/formatter_worker.py`) detecta início de frase com
  minúscula acentuada e emite mensagens em pt-BR.

## Compromissos notáveis

* **Rebase posicional, não OT/CRDT completo.** Suficiente porque um único
  primário define a ordem; muito mais simples de raciocinar e de defender. A UI
  web usa aplicação otimista com resync via REST em buracos de sequência; a
  garantia *rigorosa* de convergência é demonstrada por `sim_client.py converge`.
* **Estado autoritativo em memória + snapshots.** Cada nó mantém estado em
  memória; snapshots vão para o Redis. Um sistema de produção persistiria o log
  de forma durável; aqui os Redis Streams dão durabilidade suficiente para a demo
  e o failover.
* **Redis como coordenador.** Leases, mapa de shards e o log de replicação vivem
  no Redis, o que mantém os componentes próprios independentes da localização uns
  dos outros e torna a escala multi-EC2 uma mudança de configuração.
