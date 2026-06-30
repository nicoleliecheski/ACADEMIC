# Architecture — Shared Document Editor

## 1. Purpose

A collaborative document editor where many remote clients view and edit the same
documents at once. Edits are streamed live to every participant, and server-side
background jobs (spell-check, formatter) annotate the text concurrently. The
system is built to exercise the full set of concurrent + distributed-systems
concerns: concurrency control, partitioning, replication, consistency,
availability, synchronous and asynchronous interaction, and multi-language
components.

## 2. Component overview

```
        Internet clients  (web UI + simulated clients)
              │   WebSocket (async)        REST (sync)
              ▼
   ┌─────────────────────────────────────────────┐
   │  GATEWAY / EDGE  (Node.js)                    │
   │  REST + WebSocket terminator, sessions,       │
   │  shard ROUTER, sync RPC to primaries,         │
   │  Redis pub/sub → WebSocket fan-out            │
   └───────┬───────────────────────────┬──────────┘
     sync RPC (HTTP/JSON)         Redis pub/sub
           │                            ▲
           ▼                            │
   ┌──────────────────────────────┐    │
   │  DOCUMENT SERVICE (Python)    │    │
   │  shardA: primary  ⇄ replica   │    │
   │  shardB: primary  ⇄ replica   │    │
   │  sequencer + op-log + lease   │    │
   └───────┬──────────────────────┘    │
   Redis Streams (jobs)                 │ publish annotations
           ▼                            │
   ┌──────────────────────────────┐    │
   │  WORKERS (Python)             │────┘
   │  spell-check pool, formatter  │
   └──────────────────────────────┘
   ┌──────────────────────────────┐
   │  REDIS — pub/sub, streams,    │
   │  shard map, primary leases    │
   └──────────────────────────────┘
```

| Component | Language | Role |
|-----------|----------|------|
| Gateway / Edge | Node.js | Public REST + WebSocket endpoints; validates and routes; resolves the shard primary; fans Redis events out to WebSocket clients. |
| Document service | Python (FastAPI) | Authoritative per-shard document state; **sequencer** assigning a global per-doc `seq`; op-log; replication; lease-based primary election. |
| Background workers | Python | Spell-check and formatter pools consuming Redis Streams concurrently with editing. |
| Redis | — | Pub/Sub (notifications), Streams (job queue + replication transport), keys for shard map and leases. |
| Web UI / sim clients | JS / Python / Node | Human-visible editor + scripted clients that drive the demo. |

## 3. Interaction paradigms

* **Client–server (request/response):** browsers/clients ↔ gateway (REST + WS);
  gateway ↔ document service (HTTP/JSON RPC).
* **Publish–subscribe:** document primaries publish `op.applied`; workers publish
  annotations; the gateway subscribes and pushes to WebSocket clients.
* **Messaging / queue:** Redis Streams carry background jobs (consumer groups)
  and the replication op-log.

## 4. Data model

A document is a text snapshot plus an ordered operation log:

```
text = fold(snapshotText, opLog ordered by seq)
```

* **Edit operation (client → server):**
  `{ docId, clientId, baseVersion, op:{kind:"insert"|"delete", pos, text|len}, opId }`
* **Op-log entry (after sequencing):** the above plus `seq`, transformed `op`,
  `ts`, `appliedBy`.
* **Snapshot:** `{ docId, baseVersion, text }`, refreshed every `SNAPSHOT_EVERY`
  operations and stored in Redis (`snap:{docId}`).

## 5. Concurrency control — central sequencer + op-log

The shard **primary** is the single authoritative writer for each document and
serializes operations with a per-document lock, producing one total order
(`seq`). When a client's `baseVersion` is behind, the primary deterministically
**rebases** the operation's position against the operations sequenced in between,
then assigns the next `seq` and broadcasts the transformed operation. Because
every replica/client applies the identical transformed operations in `seq`
order, all copies converge. See `doc_service/ops.py` and `tests/test_ops.py`.

## 6. Partitioning

Documents are sharded by `docId` over a consistent-hash ring (`gateway/src/router.js`).
The shard map lives in Redis (`shardmap`). The gateway hashes `docId` → shard,
then resolves the shard's current primary from the lease key. Workers are split
into independent spell-check and formatter pools — **functional** partitioning in
addition to data partitioning.

## 7. Replication

Each shard has a primary and one or more replicas. The primary appends every
op-log entry to a Redis Stream `replog:shard:{id}`; replicas consume it in `seq`
order and apply entries idempotently (entries at/below the local head are
skipped). Reads may be served by a replica (`?replica=1`), which returns its own
possibly-stale `seq`. `REPL_MODE=sync` makes the primary wait for a replica ack
before acknowledging a write.

## 8. Availability — lease-based failover

Primary status is a Redis lease (`lease:shard:{id}`, `SET NX PX`) renewed
periodically. If the primary dies, the lease expires and a replica acquires it,
drains the replication stream, and announces `primary.changed` on
`cluster.events`. The gateway re-resolves the primary and **retries** in-flight
writes, so editing continues across a failover with continuous `seq` and no lost
operations (idempotent replay guarantees safety). See `doc_service/lease.py` and
`doc_service/replication.py`.

## 9. Synchronous vs asynchronous

* **Synchronous (blocking):** REST `POST /docs`, `GET /docs/{id}`,
  `GET /docs/{id}/ops`, `POST /docs/{id}/snapshot|save`, and the gateway→primary
  RPC.
* **Asynchronous:** WebSocket op streaming, Redis Pub/Sub fan-out of `op.applied`
  and annotations, and Redis Streams job dispatch.

## 10. Background processing

The primary marks edited documents dirty; a debounced flusher enqueues
spell-check and format jobs onto Redis Streams. Worker pools consume via consumer
groups (concurrent, at-least-once, with `XAUTOCLAIM` recovery) and publish
annotations to `doc:{docId}:annotations`, which the gateway relays to clients —
all while editing continues.

## 11. Deployment

A single EC2 host runs every component as a container via `docker-compose`
(1 gateway, 2 shards × {primary, replica}, 2 spell workers, 1 formatter, Redis).
Public endpoints: `:8080` (REST + UI) and `:8081` (WebSocket). Because all
coordination state lives in Redis, components can later be spread across multiple
EC2 instances pointed at a shared Redis. See `docs/implementation.md` and the
README for exact steps.
