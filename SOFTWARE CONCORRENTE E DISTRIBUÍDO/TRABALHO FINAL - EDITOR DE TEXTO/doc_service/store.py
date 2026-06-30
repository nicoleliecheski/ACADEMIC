"""In-memory authoritative document state with op-log and snapshots.

A ``DocState`` holds the materialized text plus the operation log since the last
snapshot. ``DocStore`` owns the collection of documents and hands out a
per-document :class:`asyncio.Lock` so that operation application is serialized
**per document** (different documents proceed concurrently).
"""

from __future__ import annotations

import asyncio
import time
from typing import Any, Dict, List, Optional

from ops import apply_op, fold

OpLogEntry = Dict[str, Any]


class DocState:
    def __init__(self, doc_id: str, shard_id: str):
        self.doc_id = doc_id
        self.shard_id = shard_id
        self.seq: int = 0                 # last applied global seq for this doc
        self.base_version: int = 0        # seq captured by snapshot_text
        self.snapshot_text: str = ""
        self.text: str = ""               # materialized current text
        self.oplog: List[OpLogEntry] = [] # entries with seq in (base_version, seq]
        self.seen_op_ids: set[str] = set()
        self.lock = asyncio.Lock()

    # -- queries -------------------------------------------------------------
    def summary(self) -> Dict[str, Any]:
        return {
            "docId": self.doc_id,
            "shardId": self.shard_id,
            "seq": self.seq,
            "baseVersion": self.base_version,
            "text": self.text,
        }

    def ops_since(self, since: int) -> Dict[str, Any]:
        """Return everything a client needs to advance from ``since`` to head.

        If ``since`` predates the current snapshot the snapshot is included so
        the client can rebuild from scratch.
        """
        if since < self.base_version:
            return {
                "docId": self.doc_id,
                "fromSnapshot": True,
                "baseVersion": self.base_version,
                "snapshotText": self.snapshot_text,
                "seq": self.seq,
                "ops": [e for e in self.oplog if e["seq"] > self.base_version],
            }
        return {
            "docId": self.doc_id,
            "fromSnapshot": False,
            "baseVersion": self.base_version,
            "seq": self.seq,
            "ops": [e for e in self.oplog if e["seq"] > since],
        }

    def intervening_ops(self, base_version: int) -> List[Dict[str, Any]]:
        """Transformed ops applied after ``base_version`` (for rebasing)."""
        return [e["op"] for e in self.oplog if e["seq"] > base_version]

    # -- mutation ------------------------------------------------------------
    def append_applied(self, entry: OpLogEntry, snapshot_every: int) -> None:
        """Record an already-sequenced, already-applied op-log entry.

        Idempotent on ``seq``: entries at or below the current head are ignored
        (this is what makes replica replay and failover safe to re-run)."""
        if entry["seq"] <= self.seq:
            return
        self.text = apply_op(self.text, entry["op"])
        self.oplog.append(entry)
        self.seq = entry["seq"]
        op_id = entry.get("opId")
        if op_id:
            self.seen_op_ids.add(op_id)
        if snapshot_every and (self.seq - self.base_version) >= snapshot_every:
            self.compact()

    def compact(self) -> None:
        """Fold the log into the snapshot and trim it."""
        self.snapshot_text = self.text
        self.base_version = self.seq
        self.oplog = [e for e in self.oplog if e["seq"] > self.base_version]

    def load_snapshot(self, base_version: int, snapshot_text: str,
                      ops: List[OpLogEntry], seq: int) -> None:
        self.base_version = base_version
        self.snapshot_text = snapshot_text
        self.oplog = list(ops)
        self.seq = seq
        self.text = fold(snapshot_text, [e["op"] for e in self.oplog])
        for e in self.oplog:
            if e.get("opId"):
                self.seen_op_ids.add(e["opId"])


class DocStore:
    def __init__(self, shard_id: str):
        self.shard_id = shard_id
        self._docs: Dict[str, DocState] = {}
        self._docs_lock = asyncio.Lock()

    def exists(self, doc_id: str) -> bool:
        return doc_id in self._docs

    def get(self, doc_id: str) -> Optional[DocState]:
        return self._docs.get(doc_id)

    async def get_or_create(self, doc_id: str) -> DocState:
        async with self._docs_lock:
            doc = self._docs.get(doc_id)
            if doc is None:
                doc = DocState(doc_id, self.shard_id)
                self._docs[doc_id] = doc
            return doc

    def all_doc_ids(self) -> List[str]:
        return list(self._docs.keys())

    @staticmethod
    def now() -> float:
        return time.time()
