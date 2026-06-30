"""Nomes centralizados de chaves / canais / streams do Redis.

Mantê-los em um só lugar garante que o gateway em Node, os serviços em Python e
os workers concordem no contrato de mais baixo nível. O lado Node espelha estes
nomes em ``gateway/src/names.js`` -- mantenha os dois em sincronia.
"""

from __future__ import annotations

# --- Chaves de coordenação --------------------------------------------------
SHARDMAP = "shardmap"


def lease_key(shard_id: str) -> str:
    return f"lease:shard:{shard_id}"


def snapshot_key(doc_id: str) -> str:
    return f"snap:{doc_id}"


# --- Streams (mensageria / filas) -------------------------------------------
def replog_stream(shard_id: str) -> str:
    """Transporte de replicação: o primário faz XADD das entradas do log; as réplicas leem com XREAD."""
    return f"replog:shard:{shard_id}"


JOBS_SPELLCHECK = "jobs:spellcheck"
JOBS_FORMAT = "jobs:format"
GROUP_SPELLERS = "spellers"
GROUP_FORMATTERS = "formatters"


# --- Canais Pub/Sub (notificações) ------------------------------------------
def doc_channel(doc_id: str) -> str:
    return f"doc:{doc_id}"


def annotations_channel(doc_id: str) -> str:
    return f"doc:{doc_id}:annotations"


def presence_channel(doc_id: str) -> str:
    return f"doc:{doc_id}:presence"


CLUSTER_EVENTS = "cluster.events"
