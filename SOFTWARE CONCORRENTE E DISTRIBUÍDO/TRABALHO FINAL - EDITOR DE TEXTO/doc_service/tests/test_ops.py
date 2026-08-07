import os
import sys

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from ops import apply_op, rebase, fold  


def test_apply_insert():
    assert apply_op("hello world", {"kind": "insert", "pos": 5, "text": ","}) == "hello, world"


def test_apply_delete():
    assert apply_op("hello world", {"kind": "delete", "pos": 5, "len": 6}) == "hello"


def test_apply_clamps_out_of_range():
    assert apply_op("abc", {"kind": "insert", "pos": 99, "text": "X"}) == "abcX"
    assert apply_op("abc", {"kind": "delete", "pos": 2, "len": 99}) == "ab"


def test_rebase_insert_after_earlier_insert():
    op = {"kind": "insert", "pos": 3, "text": "X"}
    earlier = [{"kind": "insert", "pos": 0, "text": "12"}]
    assert rebase(op, earlier)["pos"] == 5


def test_rebase_insert_after_earlier_delete():
    op = {"kind": "insert", "pos": 6, "text": "X"}
    earlier = [{"kind": "delete", "pos": 1, "len": 3}]
    assert rebase(op, earlier)["pos"] == 3


def test_rebase_tiebreak_pushes_incoming_right():
    op = {"kind": "insert", "pos": 2, "text": "B"}
    earlier = [{"kind": "insert", "pos": 2, "text": "A"}]
    assert rebase(op, earlier)["pos"] == 3


def test_concurrent_inserts_converge():
    base = "o cachorrinho"
    op1 = {"kind": "insert", "pos": 4, "text": "fofo "}
    op2 = {"kind": "insert", "pos": 4, "text": "babão "}

    t1 = apply_op(base, op1)
    op2r = rebase(op2, [op1])
    final = apply_op(t1, op2r)

    replica = fold(base, [op1, op2r])
    assert final == replica
    assert final == "o fofo babão cachorrinho"


def test_delete_then_insert_inside_deleted_region():
    base = "abcdef"
    op_del = {"kind": "delete", "pos": 1, "len": 3} 
    op_ins = {"kind": "insert", "pos": 3, "text": "X"}  
    t1 = apply_op(base, op_del)
    ins_r = rebase(op_ins, [op_del])
    assert apply_op(t1, ins_r) == fold(base, [op_del, ins_r])
