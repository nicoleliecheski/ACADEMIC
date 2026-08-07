from __future__ import annotations
import asyncio
import logging
from typing import Awaitable, Callable, Optional
import redis.asyncio as aioredis
from config import config
from names import CLUSTER_EVENTS, lease_key
import json

log = logging.getLogger("lease")

RoleCallback = Callable[[bool], Awaitable[None]]

class LeaseManager:
    def __init__(self, redis: aioredis.Redis, on_role_change: RoleCallback):
        self.redis = redis
        self.on_role_change = on_role_change
        self.is_primary: bool = False
        self.key = lease_key(config.SHARD_ID)
        self._task: Optional[asyncio.Task] = None
        self._stop = asyncio.Event()

    async def start(self) -> None:
        self._task = asyncio.create_task(self._run(), name="lease-loop")

    async def stop(self) -> None:
        self._stop.set()
        if self._task:
            await asyncio.gather(self._task, return_exceptions=True)
        if self.is_primary:
            try:
                cur = await self.redis.get(self.key)
                if cur == config.advertise_addr:
                    await self.redis.delete(self.key)
            except Exception: 
                pass

    async def _run(self) -> None:
        if config.PREFERRED_ROLE != "primary":
            try:
                await asyncio.wait_for(self._stop.wait(), timeout=config.REPLICA_START_DELAY)
                return
            except asyncio.TimeoutError:
                pass

        ttl_ms = int(config.LEASE_TTL * 1000)
        while not self._stop.is_set():
            try:
                if self.is_primary:
                    cur = await self.redis.get(self.key)
                    if cur == config.advertise_addr:
                        await self.redis.pexpire(self.key, ttl_ms)
                    else:
                        await self._become(False)
                else:
                    acquired = await self.redis.set(
                        self.key, config.advertise_addr, nx=True, px=ttl_ms
                    )
                    if acquired:
                        await self._become(True)
            except Exception as exc:  
                log.warning("lease loop error: %s", exc)

            try:
                await asyncio.wait_for(self._stop.wait(), timeout=config.LEASE_RENEW)
            except asyncio.TimeoutError:
                pass

    async def _become(self, primary: bool) -> None:
        if primary == self.is_primary:
            return
        self.is_primary = primary
        log.info("role change -> %s for shard %s",
                 "PRIMARY" if primary else "REPLICA", config.SHARD_ID)
        await self.on_role_change(primary)
        if primary:
            await self.redis.publish(CLUSTER_EVENTS, json.dumps({
                "type": "primary.changed",
                "shardId": config.SHARD_ID,
                "primary": config.advertise_addr,
            }))
