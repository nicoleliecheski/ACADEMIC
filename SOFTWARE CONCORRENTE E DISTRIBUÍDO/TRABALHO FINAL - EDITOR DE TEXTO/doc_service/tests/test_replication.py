"""Testes no estilo de integração para os invariantes de log de operações + replicação.

Rodam sem Redis e sem rede: usam as mesmas primitivas que o primário usa
(rebase -> sequenciar -> acrescentar) para construir um log de operações e então
reaplicam esse log em um store "réplica" novo, verificando a convergência.
Também cobrem o replay idempotente (a propriedade que torna o failover seguro).
"""

import os
import sys

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from ops import rebase  # noqa: E402
from store import DocState  # noqa: E402


def _primary_apply(doc: DocState, client_op, base_version, op_id):
    """Espelho do núcleo de app.rpc_op: rebaseia, sequencia, acrescenta. Retorna a entrada."""
    intervening = doc.intervening_ops(base_version)
    transformed = rebase(client_op, intervening)
    entry = {
        "type": "op", "seq": doc.seq + 1, "docId": doc.doc_id,
        "op": transformed, "opId": op_id,
    }
    doc.append_applied(entry, snapshot_every=0)
    return entry


def test_primary_sequences_concurrent_edits():
    primary = DocState("doc", "sharda")
    primary.snapshot_text = "the fox"
    primary.text = "the fox"
    # Dois clientes, ambos com base na versão 0.
    e1 = _primary_apply(primary, {"kind": "insert", "pos": 4, "text": "quick "}, 0, "c1:1")
    e2 = _primary_apply(primary, {"kind": "insert", "pos": 4, "text": "brown "}, 0, "c2:1")
    assert primary.text == "the quick brown fox"
    assert [e1["seq"], e2["seq"]] == [1, 2]
    return [e1, e2]


def test_replica_catches_up_to_primary():
    primary = DocState("doc", "sharda")
    primary.snapshot_text = "the fox"
    primary.text = "the fox"
    log = [
        _primary_apply(primary, {"kind": "insert", "pos": 4, "text": "quick "}, 0, "c1:1"),
        _primary_apply(primary, {"kind": "insert", "pos": 4, "text": "brown "}, 0, "c2:1"),
        _primary_apply(primary, {"kind": "delete", "pos": 0, "len": 4}, 2, "c1:2"),
    ]

    replica = DocState("doc", "sharda")
    replica.snapshot_text = "the fox"
    replica.text = "the fox"
    for entry in log:
        replica.append_applied(entry, snapshot_every=0)

    assert replica.text == primary.text
    assert replica.seq == primary.seq


def test_idempotent_replay():
    """Reaplicar as mesmas entradas duas vezes (como acontece em volta do failover) é inócuo."""
    primary = DocState("doc", "sharda")
    primary.snapshot_text = "abc"
    primary.text = "abc"
    log = [
        _primary_apply(primary, {"kind": "insert", "pos": 3, "text": "X"}, 0, "c1:1"),
        _primary_apply(primary, {"kind": "insert", "pos": 0, "text": "Y"}, 1, "c1:2"),
    ]

    replica = DocState("doc", "sharda")
    replica.snapshot_text = "abc"
    replica.text = "abc"
    for _ in range(2):                 # aplica todo o log duas vezes
        for entry in log:
            replica.append_applied(entry, snapshot_every=0)

    assert replica.text == primary.text == "YabcX"
    assert replica.seq == 2


def test_snapshot_compaction_preserves_text():
    doc = DocState("doc", "sharda")
    doc.snapshot_text = ""
    doc.text = ""
    for i in range(10):
        _primary_apply(doc, {"kind": "insert", "pos": 0, "text": "x"}, doc.seq, f"c:{i}")
    before = doc.text
    doc.compact()
    assert doc.snapshot_text == before
    assert doc.base_version == doc.seq
    assert doc.text == before
