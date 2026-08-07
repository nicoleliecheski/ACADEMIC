from __future__ import annotations
from typing import Any, Dict, List

Op = Dict[str, Any]

def apply_op(text: str, op: Op) -> str:
    kind = op["kind"]
    pos = max(0, min(int(op["pos"]), len(text)))
    if kind == "insert":
        ins = op.get("text", "")
        return text[:pos] + ins + text[pos:]
    if kind == "delete":
        length = max(0, int(op.get("len", 0)))
        end = min(pos + length, len(text))
        return text[:pos] + text[end:]
    raise ValueError(f"tipo de op desconhecido: {kind!r}")


def _shift_against(op: Op, earlier: Op) -> Op:
    pos = int(op["pos"])
    e_pos = int(earlier["pos"])

    if earlier["kind"] == "insert":
        e_len = len(earlier.get("text", ""))
        if e_pos <= pos:
            pos += e_len
    elif earlier["kind"] == "delete":
        e_len = max(0, int(earlier.get("len", 0)))
        if e_pos < pos:
            pos -= min(e_len, pos - e_pos)

    rebased = dict(op)
    rebased["pos"] = max(0, pos)
    return rebased


def rebase(op: Op, intervening: List[Op]) -> Op:
    rebased = dict(op)
    for earlier in intervening:
        rebased = _shift_against(rebased, earlier)
    return rebased


def fold(snapshot_text: str, ordered_ops: List[Op]) -> str:
    text = snapshot_text
    for op in ordered_ops:
        text = apply_op(text, op)
    return text
