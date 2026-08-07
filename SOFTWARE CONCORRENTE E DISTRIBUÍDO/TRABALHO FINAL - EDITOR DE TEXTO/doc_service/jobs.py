from __future__ import annotations
import asyncio
import json
import logging
from typing import Dict
import redis.asyncio as aioredis
from config import config
from names import JOBS_FORMAT, JOBS_SPELLCHECK
from store import DocStore

log = logging.getLogger("jobs")

class JobEnqueuer:
    def __init__(self, redis: aioredis.Redis, store: DocStore):
        self.redis = redis
        self.store = store
        self._dirty: Dict[str, int] = {}     
        self._flushed: Dict[str, int] = {}   
        self._task = None
        self._stop = asyncio.Event()
        self._job_seq = 0

    def mark_dirty(self, doc_id: str, seq: int) -> None:
        self._dirty[doc_id] = seq

    def start(self) -> None:
        self._task = asyncio.create_task(self._flush_loop(), name="job-flusher")

    async def stop(self) -> None:
        self._stop.set()
        if self._task:
            await asyncio.gather(self._task, return_exceptions=True)

    async def _flush_loop(self) -> None:
        while not self._stop.is_set():
            try:
                await self._flush_once()
            except Exception as exc:  
                log.warning("flush error: %s", exc)
            try:
                await asyncio.wait_for(self._stop.wait(), timeout=config.JOB_FLUSH_INTERVAL)
            except asyncio.TimeoutError:
                pass

    async def _flush_once(self) -> None:
        for doc_id, seq in list(self._dirty.items()):
            if self._flushed.get(doc_id) == seq:
                continue
            doc = self.store.get(doc_id)
            if doc is None:
                continue
            self._job_seq += 1
            job = {
                "jobId": f"{config.NODE_ID}-j{self._job_seq}",
                "docId": doc_id,
                "atSeq": doc.seq,
                "text": doc.text,
            }
            payload = {"job": json.dumps(job)}
            await self.redis.xadd(JOBS_SPELLCHECK, payload, maxlen=1000, approximate=True)
            await self.redis.xadd(JOBS_FORMAT, payload, maxlen=1000, approximate=True)
            self._flushed[doc_id] = seq
