# Implementation Notes

This document maps the design to the actual source and explains the key
algorithms and wire protocol.

## Source map

```
gateway/        Node.js edge
  src/server.js     REST API, WS server bootstrap, Redis wiring, shardmap seeding
  src/router.js     consistent-hash ring; docId→shard; primary/replica resolution
  src/rpcClient.js  HTTP/JSON RPC to doc-service; primary re-resolve + write retry
  src/redisBus.js   pub/sub subscriber → dispatch (doc events + cluster events)
  src/wsHub.js      per-doc WebSocket rooms; client message handling; fan-out
  src/names.js      Redis key/channel/stream names (mirrors doc_service/names.py)

doc_service/    Python shard node (FastAPI); same image runs primary or replica
  app.py            RPC endpoints; write pipeline; role-change orchestration
  ops.py            pure apply/rebase/fold (unit-tested)
  store.py          DocState + DocStore; op-log, snapshots, idempotency, per-doc lock
  replication.py    Redis-Stream op-log producer (primary) / consumer (replica)
  lease.py          Redis-lease primary election + heartbeat + failover callback
  jobs.py           debounced enqueue of spell-check/format jobs (Redis Streams)
  config.py         env-driven config
  names.py          Redis key/channel/stream names
  tests/            ops + replication tests (pytest or tests/run_ops.py)

workers/        Python background processors
  worker_common.py     consumer-group XREADGROUP loop + XAUTOCLAIM recovery
  spellcheck_worker.py  jobs:spellcheck → range annotations
  formatter_worker.py   jobs:format → formatting suggestions
  dictionary.py         word set + edit-distance-1 suggestions

webui/          minimal browser editor (textarea, WS client, annotations panel)
clients/        sim_client.py (asyncio) + sim_client.js (Node) simulated clients
scripts/        demo.sh, kill_primary.sh, wait_for_health.sh
test-data/      words.txt dictionary, sample docs, concurrent edit trace
```

## Write pipeline (primary) — `app.py:rpc_op`

1. `_require_primary()` — non-primaries return **409** so the gateway re-resolves.
2. Acquire the per-document `asyncio.Lock` (serializes this doc; other docs run
   concurrently).
3. **Idempotency:** if `opId` was already applied, return the prior result.
4. **Rebase:** `rebase(op, intervening_ops_since(baseVersion))`.
5. Assign `seq = doc.seq + 1`; build the op-log entry; `append_applied` (applies
   text, advances `seq`, may compact into a snapshot).
6. **Replicate:** `XADD replog:shard:{id}` (durability before ack). `REPL_MODE=sync`
   waits for a replica ack.
7. Mark the doc dirty for background jobs.
8. **Publish** `op.applied` on `doc:{docId}` (the async fan-out path).
9. Return `{seq, transformedOp}` to the gateway (the sync ack).

## Rebase algorithm — `ops.py`

For each already-sequenced operation `e` applied after the client's
`baseVersion`, shift the incoming op's position:
* `e` is an insert at `e.pos ≤ pos` → `pos += len(e.text)` (ties push the
  incoming op right, deterministically).
* `e` is a delete starting before `pos` → `pos -= min(e.len, pos − e.pos)`.

The transformed op is what gets sequenced and broadcast, so replicas and clients
that fold the same ordered transformed ops reach identical text. Proven in
`tests/test_ops.py::test_concurrent_inserts_converge` and
`tests/test_replication.py::test_replica_catches_up_to_primary`.

## Roles, leases and failover

* Role is **dynamic**: a node is primary iff it holds `lease:shard:{id}`.
* `PREFERRED_ROLE=primary` only makes that node attempt acquisition immediately;
  replicas wait `REPLICA_START_DELAY` so the intended primary wins at boot.
* On `on_role_change(primary=True)`: `drain()` the replog tail, stop consuming,
  start the job flusher, and announce `primary.changed`.
* On the gateway, a write that hits a 409 or a network error drops the primary
  cache, backs off, and retries (`rpcClient.write`, ~8s budget) — longer than a
  worst-case takeover, so writes survive the transition.

## Wire protocol

**WebSocket (client ↔ gateway).** In: `join`, `op`, `cursor`, `ping`.
Out: `resync` (full text + seq on join/gap), `op.applied`, `annotation`,
`presence`, `pong`, `error`.

**REST (client ↔ gateway).** `POST /docs`, `GET /docs/{id}[?replica=1]`,
`GET /docs/{id}/ops?since=N`, `POST /docs/{id}/snapshot`, `POST /docs/{id}/save`,
`GET /health`, `GET /shardmap`, `GET /cluster`.

**RPC (gateway → doc-service).** `POST /rpc/create`, `POST /rpc/op`,
`POST /rpc/snapshot`, `GET /rpc/doc/{id}`, `GET /rpc/ops/{id}?since=N`,
`GET /role`, `GET /health`.

**Redis.** Channels `doc:{id}`, `doc:{id}:annotations`, `doc:{id}:presence`,
`cluster.events`. Streams `replog:shard:{id}`, `jobs:spellcheck`, `jobs:format`
(groups `spellers`, `formatters`). Keys `shardmap`, `lease:shard:{id}`,
`snap:{id}`.

## Notable trade-offs

* **Positional rebase, not full OT/CRDT.** Sufficient because a single primary
  defines the order; far simpler to reason about and to defend. The web UI uses
  optimistic apply with a REST resync on sequence gaps; the *rigorous*
  convergence guarantee is demonstrated by `sim_client.py converge`.
* **In-memory authoritative state + snapshots.** Each node keeps state in memory;
  snapshots go to Redis. A production system would persist the op-log durably;
  here Redis Streams provide enough durability for the demo and failover.
* **Redis as coordinator.** Leases, shard map and the replication log all live in
  Redis, which keeps the custom components stateless about each other's location
  and makes multi-EC2 scaling a configuration change.
