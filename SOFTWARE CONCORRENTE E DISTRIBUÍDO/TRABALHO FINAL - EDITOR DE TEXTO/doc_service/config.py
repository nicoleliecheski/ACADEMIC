"""Configuração de um nó do serviço de documentos, via variáveis de ambiente.

Todo nó executa a *mesma* imagem; o comportamento é diferenciado apenas por
variáveis de ambiente. O papel primário/réplica de um nó é **dinâmico** e
decidido pela posse do lease (veja ``lease.py``); ``PREFERRED_ROLE`` apenas
enviesa quem pega o lease primeiro na inicialização, para que o primário
pretendido normalmente vença no começo.
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
    # Identidade ------------------------------------------------------------
    NODE_ID: str = os.environ.get("NODE_ID", os.environ.get("HOSTNAME", "node"))
    SHARD_ID: str = os.environ.get("SHARD_ID", "sharda")
    # Endereço pelo qual os outros componentes alcançam a API RPC deste nó.
    ADVERTISE_HOST: str = os.environ.get("ADVERTISE_HOST", os.environ.get("HOSTNAME", "localhost"))
    PORT: int = _int("PORT", 9000)
    PREFERRED_ROLE: str = os.environ.get("PREFERRED_ROLE", "replica")  # "primary"|"replica"

    # Redis -----------------------------------------------------------------
    REDIS_URL: str = os.environ.get("REDIS_URL", "redis://localhost:6379/0")

    # Replicação ------------------------------------------------------------
    # "async": o primário confirma imediatamente (leitura-da-própria-escrita no primário).
    # "sync":  o primário espera o ack de >=1 réplica antes de confirmar a escrita.
    REPL_MODE: str = os.environ.get("REPL_MODE", "async")
    SNAPSHOT_EVERY: int = _int("SNAPSHOT_EVERY", 50)

    # Lease / failover ------------------------------------------------------
    LEASE_TTL: float = _float("LEASE_TTL", 5.0)        # segundos
    LEASE_RENEW: float = _float("LEASE_RENEW", 2.0)    # segundos
    REPLICA_START_DELAY: float = _float("REPLICA_START_DELAY", 3.0)

    # Jobs de segundo plano -------------------------------------------------
    JOB_FLUSH_INTERVAL: float = _float("JOB_FLUSH_INTERVAL", 0.5)

    @property
    def advertise_addr(self) -> str:
        return f"{self.ADVERTISE_HOST}:{self.PORT}"


config = Config()
