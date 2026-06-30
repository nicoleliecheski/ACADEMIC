"""Integration-style tests for the op-log + replication invariants.

These run without Redis or the network: they drive the same primitives the
primary uses (rebase -> sequence -> append) to build an op-log, then replay that
log into a fresh "replica" store and assert convergence. Also covers idempotent
replay (the property that makes failover safe).
"""

import os
import sys

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from ops import rebase  # noqa: E402
from store import DocState  # noqa: E402


def _primary_apply(doc: DocState, client_op, base_version, op_id):
    """Mirror of app.rpc_op's core: rebase, sequence, append. Returns the entry."""
    intervening = doc.intervening_ops(base_version)
    transformed = rebase(client_op, intervening)
    entry = {
        "type": "op", "seq": doc.seq + 1, "docId": doc.doc_id,
        "op": transformed, "opId": op_id,
    }
    doc.append_applied(entry, snapshot_every=0)
    return entry


def test_primary_sequences_concurrent_edits():
    primary = DocState("doc", "shardA")
    primary.snapshot_text = "the fox"
    primary.text = "the fox"
    # Two clients both based on version 0.
    e1 = _primary_apply(primary, {"kind": "insert", "pos": 4, "text": "quick "}, 0, "c1:1")
    e2 = _primary_apply(primary, {"kind": "insert", "pos": 4, "text": "brown "}, 0, "c2:1")
    assert primary.text == "the quick brown fox"
    assert [e1["seq"], e2["seq"]] == [1, 2]
    return [e1, e2]


def test_replica_catches_up_to_primary():
    primary = DocState("doc", "shardA")
    primary.snapshot_text = "the fox"
    primary.text = "the fox"
    log = [
        _primary_apply(primary, {"kind": "insert", "pos": 4, "text": "quick "}, 0, "c1:1"),
        _primary_apply(primary, {"kind": "insert", "pos": 4, "text": "brown "}, 0, "c2:1"),
        _primary_apply(primary, {"kind": "delete", "pos": 0, "len": 4}, 2, "c1:2"),
    ]

    replica = DocState("doc", "shardA")
    replica.snapshot_text = "the fox"
    replica.text = "the fox"
    for entry in log:
        replica.append_applied(entry, snapshot_every=0)

    assert replica.text == primary.text
    assert replica.seq == primary.seq


def test_idempotent_replay():
    """Replaying the same entries twice (as happens around failover) is a no-op."""
    primary = DocState("doc", "shardA")
    primary.snapshot_text = "abc"
    primary.text = "abc"
    log = [
        _primary_apply(primary, {"kind": "insert", "pos": 3, "text": "X"}, 0, "c1:1"),
        _primary_apply(primary, {"kind": "insert", "pos": 0, "text": "Y"}, 1, "c1:2"),
    ]

    replica = DocState("doc", "shardA")
    replica.snapshot_text = "abc"
    replica.text = "abc"
    for _ in range(2):                 # apply the whole log twice
        for entry in log:
            replica.append_applied(entry, snapshot_every=0)

    assert replica.text == primary.text == "YabcX"
    assert replica.seq == 2


def test_snapshot_compaction_preserves_text():
    doc = DocState("doc", "shardA")
    doc.snapshot_text = ""
    doc.text = ""
    for i in range(10):
        _primary_apply(doc, {"kind": "insert", "pos": 0, "text": "x"}, doc.seq, f"c:{i}")
    before = doc.text
    doc.compact()
    assert doc.snapshot_text == before
    assert doc.base_version == doc.seq
    assert doc.text == before
