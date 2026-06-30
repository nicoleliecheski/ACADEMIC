"""Worker de verificação ortográfica.

Consome ``jobs:spellcheck`` (grupo de consumidores ``spellers`` — rode mais de
uma réplica para ver o consumo concorrente), procura palavras incorretas no
texto do documento e publica anotações por faixa de caracteres em
``doc:{docId}:annotations``. O gateway repassa essas anotações aos clientes via
WebSocket como eventos ``annotation``.
"""

from __future__ import annotations

import asyncio
import json
import logging
import os
import re
from typing import Dict

import redis.asyncio as aioredis

from dictionary import Dictionary
from worker_common import run_consumer

logging.basicConfig(level=logging.INFO,
                    format="%(asctime)s [speller] %(levelname)s %(message)s")
log = logging.getLogger("spellcheck")

# Reconhece palavras com letras acentuadas do português (À-ÿ cobre ç, ã, é...).
WORD_RE = re.compile(r"[A-Za-zÀ-ÿ']+")

REDIS_URL = os.environ.get("REDIS_URL", "redis://localhost:6379/0")
JOBS_SPELLCHECK = "jobs:spellcheck"
GROUP_SPELLERS = "spellers"

_pub = aioredis.from_url(REDIS_URL, decode_responses=True)
_dict = Dictionary.load()


async def handle(job: Dict) -> None:
    doc_id = job["docId"]
    text = job.get("text", "")
    issues = []
    for m in WORD_RE.finditer(text):
        token = m.group(0)
        if not _dict.is_word(token):
            issues.append({
                "range": [m.start(), m.end()],
                "word": token,
                "suggest": _dict.suggest(token),
            })
    payload = {
        "type": "spellcheck",
        "docId": doc_id,
        "atSeq": job.get("atSeq", 0),
        "issues": issues,
        "worker": os.environ.get("HOSTNAME", "speller"),
    }
    await _pub.publish(f"doc:{doc_id}:annotations", json.dumps(payload))
    log.info("doc=%s atSeq=%s issues=%d", doc_id, job.get("atSeq"), len(issues))


if __name__ == "__main__":
    asyncio.run(run_consumer(JOBS_SPELLCHECK, GROUP_SPELLERS, handle))
