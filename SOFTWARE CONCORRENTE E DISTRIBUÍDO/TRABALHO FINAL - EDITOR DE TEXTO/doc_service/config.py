"""Environment-driven configuration for a document-service node.

Every node runs the *same* image; behaviour is differentiated only by env vars.
A node's primary/replica role is **dynamic** and decided by lease ownership
(see ``lease.py``); ``PREFERRED_ROLE`` merely biases who grabs the lease first
at startup so the intended primary usually wins initially.
"""

from __future__ import annotations

import os


def _int(name: str, default: int) -> int:
    try:
        return int(os.environ.get(name, default))
    except (TypeError, ValueError):
        return default


def _float(name: str, default: float) -> float:
    try:
        return float(os.environ.get(name, default))
    except (TypeError, ValueError):
        return default


class Config:
    # Identity ---------------------------------------------------------------
    NODE_ID: str = os.environ.get("NODE_ID", os.environ.get("HOSTNAME", "node"))
    SHARD_ID: str = os.environ.get("SHARD_ID", "shardA")
    # Address other components use to reach this node's RPC API.
    ADVERTISE_HOST: str = os.environ.get("ADVERTISE_HOST", os.environ.get("HOSTNAME", "localhost"))
    PORT: int = _int("PORT", 9000)
    PREFERRED_ROLE: str = os.environ.get("PREFERRED_ROLE", "replica")  # "primary"|"replica"

    # Redis ------------------------------------------------------------------
    REDIS_URL: str = os.environ.get("REDIS_URL", "redis://localhost:6379/0")

    # Replication ------------------------------------------------------------
    # "async": primary acks immediately (read-your-writes from primary).
    # "sync":  primary waits for >=1 replica to confirm before acking.
    REPL_MODE: str = os.environ.get("REPL_MODE", "async")
    SNAPSHOT_EVERY: int = _int("SNAPSHOT_EVERY", 50)

    # Lease / failover -------------------------------------------------------
    LEASE_TTL: float = _float("LEASE_TTL", 5.0)        # seconds
    LEASE_RENEW: float = _float("LEASE_RENEW", 2.0)    # seconds
    REPLICA_START_DELAY: float = _float("REPLICA_START_DELAY", 3.0)

    # Background jobs --------------------------------------------------------
    JOB_FLUSH_INTERVAL: float = _float("JOB_FLUSH_INTERVAL", 0.5)

    @property
    def advertise_addr(self) -> str:
        return f"{self.ADVERTISE_HOST}:{self.PORT}"


config = Config()
