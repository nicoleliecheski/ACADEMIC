"""
Pure text-operation primitives and positional rebasing.

The collaborative-editing model is a *central sequencer with an operation log*:
the shard primary is the single authoritative writer for a document and applies
every operation in a total order defined by a per-document monotonic ``seq``.

This module is intentionally free of any I/O or framework code so it can be unit
tested in isolation (see ``tests/test_ops.py``). Two operation kinds are
supported:

  * insert -> ``{"kind": "insert", "pos": int, "text": str}``
  * delete -> ``{"kind": "delete", "pos": int, "len": int}``

A client submits an operation expressed against the document text it last saw
(``baseVersion``). If, by the time the operation reaches the primary, newer
operations have already been applied, the primary deterministically *rebases*
the incoming operation's position against those intervening operations before
assigning it a sequence number. Because the rebased ("transformed") operation is
what gets broadcast to every client, all replicas converge on identical text.
"""

from __future__ import annotations

from typing import Any, Dict, List

Op = Dict[str, Any]


def apply_op(text: str, op: Op) -> str:
    """Apply a single (already-positioned) operation to ``text``.

    Positions are clamped into range so a slightly stale position never raises.
    """
    kind = op["kind"]
    pos = max(0, min(int(op["pos"]), len(text)))
    if kind == "insert":
        ins = op.get("text", "")
        return text[:pos] + ins + text[pos:]
    if kind == "delete":
        length = max(0, int(op.get("len", 0)))
        end = min(pos + length, len(text))
        return text[:pos] + text[end:]
    raise ValueError(f"unknown op kind: {kind!r}")


def _shift_against(op: Op, earlier: Op) -> Op:
    """Return ``op`` with its position shifted to account for ``earlier`` having
    been applied first. ``op`` and ``earlier`` are not mutated."""
    pos = int(op["pos"])
    e_pos = int(earlier["pos"])

    if earlier["kind"] == "insert":
        e_len = len(earlier.get("text", ""))
        # An earlier insert at or before our position pushes us to the right.
        # Tie-break (e_pos == pos): the already-sequenced insert wins the spot,
        # so the incoming op moves after it -> deterministic and stable.
        if e_pos <= pos:
            pos += e_len
    elif earlier["kind"] == "delete":
        e_len = max(0, int(earlier.get("len", 0)))
        if e_pos < pos:
            # Remove the overlap between the deleted range and our offset.
            pos -= min(e_len, pos - e_pos)

    rebased = dict(op)
    rebased["pos"] = max(0, pos)
    return rebased


def rebase(op: Op, intervening: List[Op]) -> Op:
    """Rebase ``op`` over the ordered list of ``intervening`` operations that
    were sequenced after the op's ``baseVersion`` but before it.

    The result is the operation expressed against the *current* server text.
    """
    rebased = dict(op)
    for earlier in intervening:
        rebased = _shift_against(rebased, earlier)
    return rebased


def fold(snapshot_text: str, ordered_ops: List[Op]) -> str:
    """Materialize text by folding already-transformed ops over a snapshot.

    Used by replicas and by clients to reconstruct state from the op-log.
    """
    text = snapshot_text
    for op in ordered_ops:
        text = apply_op(text, op)
    return text
