"""Dicionário simples de verificação ortográfica com sugestões a distância de
edição 1.

Carrega uma lista de palavras separadas por linha (``DICT_PATH``, padrão
``/app/words.txt``). É puro e sem dependências, portanto trivial de testar.
O alfabeto de sugestões inclui letras acentuadas do português, de modo que
sugestões como ``você`` possam ser geradas a partir de ``voce``.
"""

from __future__ import annotations

import os
from typing import List, Set

# Alfabeto usado para gerar candidatos de correção (inclui acentos do pt-BR).
_ALPHABET = "abcdefghijklmnopqrstuvwxyzàáâãçéêíóôõú"


class Dictionary:
    def __init__(self, words: Set[str]):
        self.words = words

    @classmethod
    def load(cls, path: str | None = None) -> "Dictionary":
        path = path or os.environ.get("DICT_PATH", "/app/words.txt")
        words: Set[str] = set()
        try:
            with open(path, "r", encoding="utf-8") as fh:
                for line in fh:
                    w = line.strip().lower()
                    if w:
                        words.add(w)
        except FileNotFoundError:
            pass
        return cls(words)

    def is_word(self, token: str) -> bool:
        t = token.lower()
        # Aceita números puros e tokens muito curtos.
        if t.isdigit() or len(t) <= 1:
            return True
        return t in self.words

    def _edits1(self, word: str) -> Set[str]:
        splits = [(word[:i], word[i:]) for i in range(len(word) + 1)]
        deletes = [a + b[1:] for a, b in splits if b]
        transposes = [a + b[1] + b[0] + b[2:] for a, b in splits if len(b) > 1]
        replaces = [a + c + b[1:] for a, b in splits if b for c in _ALPHABET]
        inserts = [a + c + b for a, b in splits for c in _ALPHABET]
        return set(deletes + transposes + replaces + inserts)

    def suggest(self, token: str, limit: int = 3) -> List[str]:
        t = token.lower()
        cands = [w for w in self._edits1(t) if w in self.words]
        # Prioriza sugestões que começam com a mesma letra e, depois, as de
        # tamanho mais próximo.
        cands.sort(key=lambda w: (w[0] != t[0], abs(len(w) - len(t)), w))
        return cands[:limit]
