from __future__ import annotations

SHARDMAP = "shardmap"


def lease_key(shard_id: str) -> str:
    return f"lease:shard:{shard_id}"


def snapshot_key(doc_id: str) -> str:
    return f"snap:{doc_id}"


def replog_stream(shard_id: str) -> str:
    return f"replog:shard:{shard_id}"


JOBS_SPELLCHECK = "jobs:spellcheck"
JOBS_FORMAT = "jobs:format"
GROUP_SPELLERS = "spellers"
GROUP_FORMATTERS = "formatters"


def doc_channel(doc_id: str) -> str:
    return f"doc:{doc_id}"


def annotations_channel(doc_id: str) -> str:
    return f"doc:{doc_id}:annotations"


def presence_channel(doc_id: str) -> str:
    return f"doc:{doc_id}:presence"


CLUSTER_EVENTS = "cluster.events"
