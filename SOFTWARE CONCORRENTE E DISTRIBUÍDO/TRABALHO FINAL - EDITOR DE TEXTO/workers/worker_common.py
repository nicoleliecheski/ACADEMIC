"""Shared Redis Streams consumer-group plumbing for background workers.

Workers in the same group split a stream's entries between them (concurrent,
at-least-once delivery). ``XACK`` confirms processing; ``XAUTOCLAIM`` recovers
entries whose worker crashed mid-flight.
"""

from __future__ import annotations

import asyncio
import json
import logging
import os
import socket
from typing import Awaitable, Callable, Dict

import redis.asyncio as aioredis
from redis.exceptions import ResponseError

log = logging.getLogger("worker")

Handler = Callable[[Dict], Awaitable[None]]


async def ensure_group(redis: aioredis.Redis, stream: str, group: str) -> None:
    try:
        await redis.xgroup_create(stream, group, id="0", mkstream=True)
        log.info("created consumer group %s on %s", group, stream)
    except ResponseError as exc:
        if "BUSYGROUP" not in str(exc):
            raise


async def run_consumer(stream: str, group: str, handler: Handler) -> None:
    """Consume ``stream`` in ``group`` forever, dispatching each job to ``handler``."""
    redis_url = os.environ.get("REDIS_URL", "redis://localhost:6379/0")
    consumer = f"{group}-{socket.gethostname()}-{os.getpid()}"
    redis = aioredis.from_url(redis_url, decode_responses=True)
    await redis.ping()
    await ensure_group(redis, stream, group)
    log.info("worker %s consuming %s", consumer, stream)

    last_autoclaim = "0-0"
    while True:
        try:
            resp = await redis.xreadgroup(group, consumer, {stream: ">"}, count=10, block=2000)
            for _stream, entries in resp or []:
                for msg_id, fields in entries:
                    await _dispatch(redis, stream, group, msg_id, fields, handler)

            # Periodically reclaim entries abandoned by crashed workers.
            claimed = await redis.xautoclaim(stream, group, consumer,
                                              min_idle_time=10000, start=last_autoclaim, count=10)
            if claimed and claimed[1]:
                last_autoclaim = claimed[0]
                for msg_id, fields in claimed[1]:
                    await _dispatch(redis, stream, group, msg_id, fields, handler)
        except asyncio.CancelledError:  # pragma: no cover
            raise
        except Exception as exc:  # pragma: no cover - keep the worker alive
            log.warning("consumer loop error: %s", exc)
            await asyncio.sleep(0.5)


async def _dispatch(redis, stream, group, msg_id, fields, handler) -> None:
    raw = fields.get("job")
    if not raw:
        await redis.xack(stream, group, msg_id)
        return
    try:
        await handler(json.loads(raw))
    except Exception as exc:  # pragma: no cover
        log.warning("handler error on %s: %s", msg_id, exc)
    finally:
        # At-least-once: ack after handling (a crash before ack -> reprocessed).
        await redis.xack(stream, group, msg_id)
