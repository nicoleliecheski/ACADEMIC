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
        self.seq: int = 0                 
        self.base_version: int = 0        
        self.snapshot_text: str = ""
        self.text: str = ""               
        self.oplog: List[OpLogEntry] = [] 
        self.seen_op_ids: set[str] = set()
        self.lock = asyncio.Lock()

    def summary(self) -> Dict[str, Any]:
        return {
            "docId": self.doc_id,
            "shardId": self.shard_id,
            "seq": self.seq,
            "baseVersion": self.base_version,
            "text": self.text,
        }

    def ops_since(self, since: int) -> Dict[str, Any]:
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
        return [e["op"] for e in self.oplog if e["seq"] > base_version]

    def append_applied(self, entry: OpLogEntry, snapshot_every: int) -> None:
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
