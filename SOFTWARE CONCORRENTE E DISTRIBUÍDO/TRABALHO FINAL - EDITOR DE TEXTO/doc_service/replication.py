"""Op-log replication over a Redis Stream (the replication transport).

The shard's op-log is a single Redis Stream ``replog:shard:{shardId}``:

  * the **primary** XADDs each sequenced op-log entry (and ``create`` control
    entries) after applying it locally;
  * **replicas** XREAD the stream in order and apply each entry idempotently
    (entries whose ``seq`` is at or below the doc head are skipped).

A node tracks ``last_id`` for the stream across role flips. When a replica is
promoted it first *drains* any outstanding entries, then begins producing -- so
no operation is lost across a failover and ``seq`` stays continuous.
"""

from __future__ import annotations

import asyncio
import json
import logging
from typing import Any, Dict, Optional

import redis.asyncio as aioredis

from config import config
from names import replog_stream
from store import DocStore

log = logging.getLogger("replication")


class Replicator:
    def __init__(self, redis: aioredis.Redis, store: DocStore):
        self.redis = redis
        self.store = store
        self.stream = replog_stream(config.SHARD_ID)
        self.last_id: str = "0-0"
        self._consumer_task: Optional[asyncio.Task] = None
        self._consuming = asyncio.Event()

    # -- producer side (primary) --------------------------------------------
    async def produce(self, entry: Dict[str, Any]) -> int:
        """Append an op-log / control entry to the stream. Returns # acks (sync)."""
        msg_id = await self.redis.xadd(self.stream, {"entry": json.dumps(entry)})
        self.last_id = msg_id
        if config.REPL_MODE == "sync":
            return await self._await_replica_catchup(entry.get("seq", 0))
        return 0

    async def _await_replica_catchup(self, target_seq: int, timeout: float = 2.0) -> int:
        """Best-effort wait until a replica has acknowledged ``target_seq``.

        Replicas publish their applied head to ``replack:shard:{shard}`` (a simple
        hash). This keeps REPL_MODE=sync honest without a full consensus layer.
        """
        deadline = asyncio.get_event_loop().time() + timeout
        key = f"replack:shard:{config.SHARD_ID}"
        while asyncio.get_event_loop().time() < deadline:
            acks = await self.redis.hgetall(key)
            confirmed = sum(1 for v in acks.values() if int(v) >= target_seq)
            if confirmed >= 1:
                return confirmed
            await asyncio.sleep(0.02)
        return 0

    # -- consumer side (replica) --------------------------------------------
    def start_consuming(self) -> None:
        if self._consumer_task is None or self._consumer_task.done():
            self._consuming.set()
            self._consumer_task = asyncio.create_task(
                self._consume_loop(), name="replog-consumer")

    async def stop_consuming(self) -> None:
        self._consuming.clear()
        if self._consumer_task:
            await asyncio.gather(self._consumer_task, return_exceptions=True)
            self._consumer_task = None

    async def drain(self) -> None:
        """Apply all currently-available entries (used right before promotion)."""
        while True:
            resp = await self.redis.xread({self.stream: self.last_id}, count=500, block=10)
            if not resp:
                return
            await self._apply_batch(resp)

    async def _consume_loop(self) -> None:
        ackkey = f"replack:shard:{config.SHARD_ID}"
        while self._consuming.is_set():
            try:
                resp = await self.redis.xread({self.stream: self.last_id}, count=200, block=500)
                if resp:
                    await self._apply_batch(resp)
                    # Report our applied head for REPL_MODE=sync acks.
                    heads = [self.store.get(d).seq for d in self.store.all_doc_ids()
                             if self.store.get(d)]
                    if heads:
                        await self.redis.hset(ackkey, config.NODE_ID, max(heads))
            except asyncio.CancelledError:  # pragma: no cover
                raise
            except Exception as exc:  # pragma: no cover - transient
                log.warning("replog consume error: %s", exc)
                await asyncio.sleep(0.2)

    async def _apply_batch(self, resp) -> None:
        for _stream, entries in resp:
            for msg_id, fields in entries:
                self.last_id = msg_id
                raw = fields.get("entry")
                if not raw:
                    continue
                entry = json.loads(raw)
                await self._apply_entry(entry)

    async def _apply_entry(self, entry: Dict[str, Any]) -> None:
        etype = entry.get("type", "op")
        doc_id = entry["docId"]
        if etype == "create":
            await self.store.get_or_create(doc_id)
            return
        doc = await self.store.get_or_create(doc_id)
        async with doc.lock:
            doc.append_applied(entry, config.SNAPSHOT_EVERY)
