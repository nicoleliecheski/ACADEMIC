"""Eleição de primário via um lease no Redis (o mecanismo de disponibilidade).

Cada shard tem exatamente uma chave de lease ``lease:shard:{shardId}``. Quem a
detém é o **primário**; todos os demais são réplicas. O detentor renova o lease
antes do seu TTL expirar. Se o primário cair, o lease expira e uma réplica o
adquire (``SET NX``) e é promovida. O nó com ``PREFERRED_ROLE=primary`` tenta
adquirir imediatamente, enquanto réplicas esperam ``REPLICA_START_DELAY``
segundos, de modo que o primário pretendido normalmente vença no boot.

Um pequeno callback dispara a cada transição de papel, para que o nó alterne
entre produzir o log de replicação (primário) e consumi-lo (réplica).
"""

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
        # Libera o lease no melhor esforço, para um standby assumir de imediato.
        if self.is_primary:
            try:
                cur = await self.redis.get(self.key)
                if cur == config.advertise_addr:
                    await self.redis.delete(self.key)
            except Exception:  # pragma: no cover - melhor esforço no desligamento
                pass

    async def _run(self) -> None:
        if config.PREFERRED_ROLE != "primary":
            # Dá ao primário preferido uma vantagem no boot a frio.
            try:
                await asyncio.wait_for(self._stop.wait(), timeout=config.REPLICA_START_DELAY)
                return
            except asyncio.TimeoutError:
                pass

        ttl_ms = int(config.LEASE_TTL * 1000)
        while not self._stop.is_set():
            try:
                if self.is_primary:
                    # Renova só se ainda formos donos (evita split brain).
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
            except Exception as exc:  # pragma: no cover - erros transitórios do redis
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
            # Anuncia para que os gateways re-resolvam o roteamento deste shard.
            await self.redis.publish(CLUSTER_EVENTS, json.dumps({
                "type": "primary.changed",
                "shardId": config.SHARD_ID,
                "primary": config.advertise_addr,
            }))
