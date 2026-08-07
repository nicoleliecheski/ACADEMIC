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
    NODE_ID: str = os.environ.get("NODE_ID", os.environ.get("HOSTNAME", "node"))
    SHARD_ID: str = os.environ.get("SHARD_ID", "sharda")
    ADVERTISE_HOST: str = os.environ.get("ADVERTISE_HOST", os.environ.get("HOSTNAME", "localhost"))
    PORT: int = _int("PORT", 9000)
    PREFERRED_ROLE: str = os.environ.get("PREFERRED_ROLE", "replica")

    REDIS_URL: str = os.environ.get("REDIS_URL", "redis://localhost:6379/0")

    REPL_MODE: str = os.environ.get("REPL_MODE", "async")
    SNAPSHOT_EVERY: int = _int("SNAPSHOT_EVERY", 50)

    LEASE_TTL: float = _float("LEASE_TTL", 5.0)       
    LEASE_RENEW: float = _float("LEASE_RENEW", 2.0)    
    REPLICA_START_DELAY: float = _float("REPLICA_START_DELAY", 3.0)

    JOB_FLUSH_INTERVAL: float = _float("JOB_FLUSH_INTERVAL", 0.5)

    @property
    def advertise_addr(self) -> str:
        return f"{self.ADVERTISE_HOST}:{self.PORT}"


config = Config()
