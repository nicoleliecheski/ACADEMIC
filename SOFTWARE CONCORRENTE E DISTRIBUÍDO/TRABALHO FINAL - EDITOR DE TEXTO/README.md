# Course Final Project Design and Implementation of a Concurrent and Distributed Software System

## Objective
The objective of the project is to exercise, in an integrated manner, the concepts of distributed systems and concurrent programming in the construction of a software system. The project must explore methods and patterns to solve the main problems of concurrency and distribution, making use of currently relevant technologies and tools.

## Overview
The system to be developed must contain elements of distributed systems and concurrent programming, involving different programming models (with the use of more than one programming language) and interaction paradigms (client-server, publish-subscribe, messaging).

**The system must contain the following characteristics, regardless of the application scenario:**
- **Service accessible to multiple clients on the Internet;**
- **Service constituted through the integration and coordination of several distributed components, which must be implemented as part of the project;**
- **Concurrent accesses to shared resources/data;**
- **Data processing on the server side, concurrently with client accesses;**
- **Use of synchronous (blocking) and asynchronous remote interaction mechanisms;**
- **Replication and partitioning of data and functionalities;**
- **Treatments to ensure data consistency and availability of functionalities.**

The elaboration of the specific application scenario, including requirements and architecture, is an integral part of the project.

## Examples of Application Scenarios
- Shared database, which can be accessed simultaneously by different remote clients, with operations for data access (minimally, CRUD), event notification (e.g., related to performance parameters) to an administration dashboard, and automatic maintenance operations (e.g., sorting, data sanitization, consistency, etc.).
- **Shared document editor, with functionalities for remote visualization and editing by several simultaneous clients, notification of editing events to the other clients, and background processing operations (e.g., spell checker, text formatter).**
- Multiplayer online game, in which multiple players can simultaneously view the shared state of the game, execute actions that modify this state, and receive notifications of state changes made by other players or by internal game rule maintenance operations.
- Inventory tracking system, in which multiple sellers and buyers can simultaneously perform checkout (sale) or check-in (purchase) operations of products, with support for alerts (e.g., low quantity products) and for internal inventory maintenance operations (e.g., for quantity reconciliation after losses).

## Format
- **Implementation of the system services with the characteristics described above, along with simulated clients to demonstrate their use.**
- **Create a representative demonstration scenario that allows systematically exercising the system's characteristics.**
- **Execute the demonstration using the AWS cloud (EC2).**
- **Artifacts to be delivered: source code (and executables); documentation (of architecture and implementation); usage instructions (readme); and test data.**

---

# Implementação: Editor de Documentos Colaborativo

Um editor de documentos em que vários clientes remotos visualizam e editam os
mesmos documentos ao mesmo tempo. As edições são transmitidas ao vivo a cada
participante via WebSocket, e workers de segundo plano em Python (corretor
ortográfico, formatador) anotam o texto concorrentemente com a edição. O sistema
é construído a partir de componentes distribuídos próprios em **duas linguagens**
(Node.js + Python) coordenados através do **Redis**. O corretor ortográfico
verifica **português do Brasil**.

* Arquitetura: [`docs/architecture.md`](docs/architecture.md)
* Detalhes de implementação / protocolo: [`docs/implementation.md`](docs/implementation.md)

## Como as características exigidas são atendidas

| Requisito | Onde |
|-----------|------|
| Múltiplos clientes na Internet | Gateway REST (`:8080`) + WebSocket (`:8081`) |
| Vários componentes distribuídos próprios | Gateway Node, doc-service Python (primário/réplica), workers Python, coordenador de lease |
| Acesso concorrente a dados compartilhados | Vários clientes editando um doc; trava por documento + sequenciador |
| Processamento no servidor concorrente com o acesso | Pools de corretor/formatação (grupos de consumidores do Redis Streams) |
| Interação síncrona **e** assíncrona | REST bloqueante + RPC gateway→primário; WebSocket + Redis Pub/Sub + Streams |
| Replicação **e** particionamento | Shards por hash consistente; stream de log primário→réplica; pools de workers separados |
| Consistência **e** disponibilidade | Sequenciador central + log de ops + rebase; failover por lease + retry de escrita |
| >1 linguagem, múltiplos paradigmas | Node + Python; cliente-servidor, pub/sub, mensageria |

## Arquitetura num relance

```
clientes ──WS(assíncrono)/REST(síncrono)──> gateway Node ──RPC──> doc-service Python
                                       │  ▲                 (sharda/b: primário⇄réplica)
                                       │  └── Redis Pub/Sub ──┐ op.applied / annotations
                                       └── roteador de shards  │
workers Python <── Redis Streams (jobs) ── doc-service ────────┘
Redis: pub/sub, streams, mapa de shards, leases de primário
```

## Pré-requisitos

* Docker + Docker Compose v2 (`docker compose`).
* Para os clientes simulados / demo na máquina: Python 3.9+ com o pacote
  `websockets` (`pip install -r clients/requirements.txt`). O script de demo o
  instala automaticamente se faltar.

## Rodar localmente

```bash
make up        # builda as imagens, sobe o cluster e espera ficar saudável
make ps        # lista todos os containers
make cluster   # mostra os papéis primário/réplica por shard, ao vivo
make demo      # roda o cenário de demonstração completo e narrado
make down      # para tudo
```

Depois abra a UI web em <http://localhost:8080>. Abra em **duas abas do
navegador** com o mesmo id de documento (ex.: `doc-A`) e digite nas duas para ver
a colaboração ao vivo, além das anotações de ortografia/formatação no painel
lateral.

### Endpoints úteis

```bash
curl localhost:8080/health
curl localhost:8080/shardmap
curl localhost:8080/cluster            # quem é primário/réplica agora
curl -X POST localhost:8080/docs -H 'content-type: application/json' -d '{"docId":"doc-A"}'
curl localhost:8080/docs/doc-A
curl 'localhost:8080/docs/doc-A?replica=1'   # leitura servida por uma réplica
```

### Clientes simulados

Use `python3` (na maioria das distros Linux, incluindo Amazon Linux 2023, não
existe o `python` puro). Instale a única dependência primeiro:

```bash
python3 -m pip install --user websockets   # acrescente --break-system-packages se o pip recusar
python3 clients/sim_client.py create   --doc doc-A
python3 clients/sim_client.py converge --doc doc-A --clients 5 --ops 12   # prova de consistência
python3 clients/sim_client.py type     --doc doc-A --client t1 --text "voce escreveu rapido"
python3 clients/sim_client.py watch    --doc doc-A --seconds 10           # ver anotações
```

O cliente Node.js é **opcional** (o cliente Python cobre o mesmo comportamento;
o `node` não vem instalado na máquina por padrão, pois roda dentro de
containers). Para usá-lo, instale o Node e sua dependência primeiro:

```bash
sudo dnf -y install nodejs        # Amazon Linux 2023  (Ubuntu: sudo apt-get install -y nodejs npm)
( cd clients && npm install )     # instala o pacote 'ws'
node clients/sim_client.js edit --doc doc-A --client n1 --ops 15
```

### Demonstrar disponibilidade (failover)

```bash
scripts/kill_primary.sh sharda   # derruba o primário atual; uma réplica é promovida
make cluster                     # confirma o novo primário
# a edição continua, com seq contínuo e sem ops perdidas
```

## Rodar os testes de unidade/integração

```bash
make test    # ordenação do sequenciador, corretude do rebase, catch-up da réplica, idempotência
```

## Implantar na AWS EC2

1. Crie uma instância EC2 (Amazon Linux 2023 ou Ubuntu, t3.small+).
2. **Entrada do security group:** TCP `8080` (REST + UI) e `8081` (WebSocket) a
   partir do seu IP (ou `0.0.0.0/0` para uma demo aberta). Mantenha `6379` e
   `9000` fechados — o Redis e os nós de shard ficam na rede interna do Docker.
3. Instale Docker + Compose, copie este projeto para a instância e então:
   ```bash
   cd "TRABALHO FINAL - EDITOR DE TEXTO"
   make up
   ```
4. Do seu computador, acesse `http://<IP_PUBLICO_EC2>:8080`. Para apontar os
   clientes simulados para a instância:
   ```bash
   GW_HTTP=http://<IP_PUBLICO_EC2>:8080 GW_WS=ws://<IP_PUBLICO_EC2>:8081 \
     python3 clients/sim_client.py converge --doc doc-A --clients 5 --ops 12
   ```
5. Rode `make demo` na instância para exercitar cada característica ponta a ponta.

Como toda a coordenação (leases, mapa de shards, log de replicação) vive no
Redis, os shards/réplicas/workers podem depois ser distribuídos em várias
instâncias EC2 apontando para um Redis compartilhado, com um balanceador de carga
na frente de múltiplos gateways.

### Passo a passo no AWS Academy Learner Lab

O **AWS Academy Learner Lab** (gratuito) funciona para este projeto. Ele é
baseado em sessão: clique em **Start Lab** no curso (espere o ponto 🟢) e depois
em **AWS** para abrir o console. A região é fixa (geralmente **us-east-1**) e a
instância fica *parada* (não apagada) ao fim da sessão — basta dar **Start** de
novo na próxima sessão, e note que o **IP público muda** a cada vez.

1. **Crie a instância.** EC2 → **Launch instances**:
   - AMI: *Amazon Linux 2023*; Tipo: **t3.small** (ou t3.medium); Disco: **20 GB**.
   - Par de chaves: crie um e baixe o `.pem`, *ou* pule isso e use o
     **EC2 Instance Connect** (SSH pelo navegador) no passo 3.
2. **Abra as portas (regras de entrada do security group):**

   | Tipo | Porta | Origem |
   |------|-------|--------|
   | SSH | 22 | `0.0.0.0/0` |
   | Custom TCP | 8080 | `0.0.0.0/0` |
   | Custom TCP | 8081 | `0.0.0.0/0` |

   (`0.0.0.0/0` = aberto a qualquer um; ok para uma demo curta de aula. Deixe 6379 / 9000 fechadas.)
3. **Conecte:** selecione a instância → **Connect** → **EC2 Instance Connect** → **Connect**.
4. **Instale o Docker, clone (este repo é público — sem token) e rode:**
   ```bash
   sudo dnf -y install git
   git clone --branch task-ver-2-editor-em-pt-br https://github.com/nicoleliecheski/ACADEMIC.git
   cd "ACADEMIC/SOFTWARE CONCORRENTE E DISTRIBUÍDO/TRABALHO FINAL - EDITOR DE TEXTO"
   bash scripts/aws_ec2_setup.sh    # instala Docker + Compose + buildx
   newgrp docker                    # usar docker sem relogar
   make up                          # builda + sobe o cluster (~2-3 min na 1ª vez)
   make demo                        # roda a demonstração completa
   ```
   Os nomes das pastas têm espaços/acento, então o caminho do `cd` **precisa** estar entre aspas.
5. **Abra a UI** em `http://<IP_PUBLICO_EC2>:8080` (o IPv4 público está na página
   *Instances* do EC2). Abra duas abas com o mesmo id de documento para mostrar a
   colaboração ao vivo.
6. **Ao terminar:** EC2 → selecione a instância → **Instance state → Stop**
   (economiza orçamento; dê Start de novo na próxima sessão).

## Estrutura do repositório

Veja [`docs/implementation.md`](docs/implementation.md) para o mapa completo do
código-fonte. No topo: `gateway/` (Node), `doc_service/` (Python), `workers/`
(Python), `webui/`, `clients/`, `scripts/`, `test-data/`, `docs/`,
`docker-compose.yml`, `Makefile`.
