"""Centralized Redis key / channel / stream names.

Keeping these in one place guarantees the Node gateway, the Python services and
the workers all agree on the wire-level contract. The Node side mirrors these in
``gateway/src/names.js`` -- keep the two in sync.
"""

from __future__ import annotations

# --- Coordination keys ------------------------------------------------------
SHARDMAP = "shardmap"


def lease_key(shard_id: str) -> str:
    return f"lease:shard:{shard_id}"


def snapshot_key(doc_id: str) -> str:
    return f"snap:{doc_id}"


# --- Streams (messaging / queues) -------------------------------------------
def replog_stream(shard_id: str) -> str:
    """Replication transport: primary XADDs op-log entries, replicas XREAD."""
    return f"replog:shard:{shard_id}"


JOBS_SPELLCHECK = "jobs:spellcheck"
JOBS_FORMAT = "jobs:format"
GROUP_SPELLERS = "spellers"
GROUP_FORMATTERS = "formatters"


# --- Pub/Sub channels (notifications) ---------------------------------------
def doc_channel(doc_id: str) -> str:
    return f"doc:{doc_id}"


def annotations_channel(doc_id: str) -> str:
    return f"doc:{doc_id}:annotations"


def presence_channel(doc_id: str) -> str:
    return f"doc:{doc_id}:presence"


CLUSTER_EVENTS = "cluster.events"
