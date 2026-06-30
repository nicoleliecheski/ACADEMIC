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

# Implementation: Shared Collaborative Document Editor

A document editor where many remote clients view and edit the same documents at
once. Edits are streamed live to every participant over WebSocket, and Python
background workers (spell-check, formatter) annotate the text concurrently with
editing. The system is built from custom distributed components in **two
languages** (Node.js + Python) coordinated through **Redis**.

* Architecture: [`docs/architecture.md`](docs/architecture.md)
* Implementation details / wire protocol: [`docs/implementation.md`](docs/implementation.md)

## How the required characteristics are met

| Requirement | Where |
|-------------|-------|
| Multiple Internet clients | Gateway REST (`:8080`) + WebSocket (`:8081`) |
| Several custom distributed components | Node gateway, Python doc-service (primary/replica), Python workers, lease coordinator |
| Concurrent access to shared data | Many clients editing one doc; per-doc lock + sequencer |
| Server-side processing concurrent with access | Spell-check/format worker pools (Redis Streams consumer groups) |
| Synchronous **and** asynchronous interaction | Blocking REST + gateway→primary RPC; WebSocket + Redis Pub/Sub + Streams |
| Replication **and** partitioning | Consistent-hash shards; primary→replica op-log stream; separate worker pools |
| Consistency **and** availability | Central sequencer + op-log + rebase; lease failover + write retry |
| >1 language, multiple paradigms | Node + Python; client-server, pub/sub, messaging |

## Architecture at a glance

```
clients ──WS(async)/REST(sync)──> Node gateway ──RPC──> Python doc-service
                                       │  ▲                 (shardA/B: primary⇄replica)
                                       │  └── Redis Pub/Sub ──┐ op.applied / annotations
                                       └── shard router        │
Python workers <── Redis Streams (jobs) ── doc-service ────────┘
Redis: pub/sub, streams, shard map, primary leases
```

## Prerequisites

* Docker + Docker Compose v2 (`docker compose`).
* For the simulated clients / demo on the host: Python 3.9+ with the
  `websockets` package (`pip install -r clients/requirements.txt`). The demo
  script installs it automatically if missing.

## Run locally

```bash
make up        # build images, start the cluster, wait until healthy
make ps        # see all containers
make cluster   # show live primary/replica roles per shard
make demo      # run the full narrated demonstration scenario
make down      # stop everything
```

Then open the web UI at <http://localhost:8080>. Open it in **two browser tabs**
with the same Doc id (e.g. `doc-A`) and type in both to see live collaboration,
plus spell-check/formatter annotations in the side panel.

### Useful endpoints

```bash
curl localhost:8080/health
curl localhost:8080/shardmap
curl localhost:8080/cluster            # who is primary/replica right now
curl -X POST localhost:8080/docs -H 'content-type: application/json' -d '{"docId":"doc-A"}'
curl localhost:8080/docs/doc-A
curl 'localhost:8080/docs/doc-A?replica=1'   # read served by a replica
```

### Simulated clients

```bash
pip install -r clients/requirements.txt
python clients/sim_client.py create   --doc doc-A
python clients/sim_client.py converge --doc doc-A --clients 5 --ops 12   # consistency proof
python clients/sim_client.py type     --doc doc-A --client t1 --text "teh quik fox"
python clients/sim_client.py watch    --doc doc-A --seconds 10           # see annotations
node   clients/sim_client.js edit     --doc doc-A --client n1 --ops 15   # Node client
```

### Demonstrate availability (failover)

```bash
scripts/kill_primary.sh shardA   # kills the current primary; a replica is promoted
make cluster                     # confirm the new primary
# editing continues with continuous seq and no lost ops
```

## Run the unit/integration tests

```bash
make test    # sequencer ordering, rebase correctness, replica catch-up, idempotency
```

## Deploy on AWS EC2

1. Launch an EC2 instance (Amazon Linux 2023 or Ubuntu, t3.small+).
2. **Security group inbound:** TCP `8080` (REST + UI) and `8081` (WebSocket) from
   your IP (or `0.0.0.0/0` for an open demo). Keep `6379` and `9000` closed —
   Redis and the shard nodes stay on the internal Docker network.
3. Install Docker + Compose, copy this project to the instance, then:
   ```bash
   cd "TRABALHO FINAL - EDITOR DE TEXTO"
   make up
   ```
4. From your laptop, browse to `http://<EC2_PUBLIC_IP>:8080`. To point the
   simulated clients at the instance:
   ```bash
   GW_HTTP=http://<EC2_PUBLIC_IP>:8080 GW_WS=ws://<EC2_PUBLIC_IP>:8081 \
     python clients/sim_client.py converge --doc doc-A --clients 5 --ops 12
   ```
5. Run `make demo` on the instance to exercise every characteristic end-to-end.

Because all coordination (leases, shard map, replication log) lives in Redis, the
shards/replicas/workers can later be split across several EC2 instances pointed
at a shared Redis, with a load balancer in front of multiple gateways.

### Step-by-step on the AWS Academy Learner Lab

The free **AWS Academy Learner Lab** works for this project. It is session-based:
click **Start Lab** in the course (wait for the 🟢 dot), then **AWS** to open the
console. The region is fixed (usually **us-east-1**) and the instance is *stopped*
(not deleted) when the session ends — just **Start** it again next session, and
note that its **public IP changes** each time.

1. **Launch the instance.** EC2 → **Launch instances**:
   - AMI: *Amazon Linux 2023*; Instance type: **t3.small** (or t3.medium); Storage: **20 GB**.
   - Key pair: create one and download the `.pem`, *or* skip it and use
     **EC2 Instance Connect** (browser SSH) in step 3.
2. **Open ports (security group inbound rules):**

   | Type | Port | Source |
   |------|------|--------|
   | SSH | 22 | `0.0.0.0/0` |
   | Custom TCP | 8080 | `0.0.0.0/0` |
   | Custom TCP | 8081 | `0.0.0.0/0` |

   (`0.0.0.0/0` = open to anyone; fine for a short-lived class demo. Leave 6379 / 9000 closed.)
3. **Connect:** select the instance → **Connect** → **EC2 Instance Connect** → **Connect**.
4. **Install Docker, clone (this repo is public — no token needed), and run:**
   ```bash
   sudo dnf -y install git
   git clone --branch task-ver-1 https://github.com/nicoleliecheski/ACADEMIC.git
   cd "ACADEMIC/SOFTWARE CONCORRENTE E DISTRIBUÍDO/TRABALHO FINAL - EDITOR DE TEXTO"
   bash scripts/aws_ec2_setup.sh    # installs Docker + Compose
   newgrp docker                    # use docker without re-login
   make up                          # build + start the cluster (~2-3 min first time)
   make demo                        # run the full demonstration
   ```
   The folder names contain spaces/an accent, so the `cd` path **must** be quoted.
5. **Open the UI** at `http://<EC2_PUBLIC_IP>:8080` (find the Public IPv4 address on
   the EC2 *Instances* page). Open two browser tabs with the same Doc id to show
   live collaboration.
6. **When done:** EC2 → select instance → **Instance state → Stop** (saves budget;
   Start it again next session).

## Repository layout

See [`docs/implementation.md`](docs/implementation.md) for a full source map.
Top level: `gateway/` (Node), `doc_service/` (Python), `workers/` (Python),
`webui/`, `clients/`, `scripts/`, `test-data/`, `docs/`, `docker-compose.yml`,
`Makefile`.
