"""Primitivas puras de operação de texto e rebaseamento posicional.

O modelo de edição colaborativa é um *sequenciador central com um log de
operações*: o primário do shard é o único escritor autoritativo de um documento
e aplica cada operação em uma ordem total definida por um ``seq`` monotônico por
documento.

Este módulo é propositalmente livre de I/O ou código de framework, para poder
ser testado isoladamente (veja ``tests/test_ops.py``). Há suporte a dois tipos
de operação:

  * insert -> ``{"kind": "insert", "pos": int, "text": str}``
  * delete -> ``{"kind": "delete", "pos": int, "len": int}``

Um cliente envia uma operação expressa em relação ao texto que viu por último
(``baseVersion``). Se, quando a operação chega ao primário, operações mais novas
já tiverem sido aplicadas, o primário **rebaseia** de forma determinística a
posição da operação recebida sobre essas operações intermediárias antes de lhe
atribuir um número de sequência. Como a operação rebaseada ("transformada") é a
que é difundida a todos os clientes, todas as réplicas convergem para o mesmo
texto.
"""

from __future__ import annotations

from typing import Any, Dict, List

Op = Dict[str, Any]


def apply_op(text: str, op: Op) -> str:
    """Aplica uma única operação (já posicionada) a ``text``.

    As posições são limitadas ao intervalo válido, de modo que uma posição
    levemente desatualizada nunca cause exceção.
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
    raise ValueError(f"tipo de op desconhecido: {kind!r}")


def _shift_against(op: Op, earlier: Op) -> Op:
    """Retorna ``op`` com a posição deslocada para considerar que ``earlier`` foi
    aplicada antes. ``op`` e ``earlier`` não são modificados."""
    pos = int(op["pos"])
    e_pos = int(earlier["pos"])

    if earlier["kind"] == "insert":
        e_len = len(earlier.get("text", ""))
        # Uma inserção anterior em posição <= à nossa nos empurra para a direita.
        # Desempate (e_pos == pos): a inserção já sequenciada fica com o lugar, e
        # a operação recebida vai para depois dela -> determinístico e estável.
        if e_pos <= pos:
            pos += e_len
    elif earlier["kind"] == "delete":
        e_len = max(0, int(earlier.get("len", 0)))
        if e_pos < pos:
            # Remove a sobreposição entre a faixa apagada e o nosso deslocamento.
            pos -= min(e_len, pos - e_pos)

    rebased = dict(op)
    rebased["pos"] = max(0, pos)
    return rebased


def rebase(op: Op, intervening: List[Op]) -> Op:
    """Rebaseia ``op`` sobre a lista ordenada de operações ``intervening`` que
    foram sequenciadas após o ``baseVersion`` da op, mas antes dela.

    O resultado é a operação expressa em relação ao texto *atual* do servidor.
    """
    rebased = dict(op)
    for earlier in intervening:
        rebased = _shift_against(rebased, earlier)
    return rebased


def fold(snapshot_text: str, ordered_ops: List[Op]) -> str:
    """Materializa o texto dobrando ops já transformadas sobre um snapshot.

    Usado por réplicas e por clientes para reconstruir o estado a partir do log.
    """
    text = snapshot_text
    for op in ordered_ops:
        text = apply_op(text, op)
    return text
