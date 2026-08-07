from __future__ import annotations

import asyncio
import json
import logging
import os
import re
from typing import Dict

import redis.asyncio as aioredis

from worker_common import run_consumer

logging.basicConfig(level=logging.INFO,
                    format="%(asctime)s [formatter] %(levelname)s %(message)s")
log = logging.getLogger("formatter")

REDIS_URL = os.environ.get("REDIS_URL", "redis://localhost:6379/0")
JOBS_FORMAT = "jobs:format"
GROUP_FORMATTERS = "formatters"

_pub = aioredis.from_url(REDIS_URL, decode_responses=True)

DOUBLE_SPACE = re.compile(r"  +")
TRAILING_WS = re.compile(r"[ \t]+$", re.MULTILINE)
SENTENCE_START = re.compile(r"(?:^|[.!?]\s+)([a-zà-ÿ])")


async def handle(job: Dict) -> None:
    doc_id = job["docId"]
    text = job.get("text", "")
    suggestions = []

    for m in DOUBLE_SPACE.finditer(text):
        suggestions.append({"range": [m.start(), m.end()], "kind": "double-space",
                            "message": "juntar em um único espaço"})
    for m in TRAILING_WS.finditer(text):
        suggestions.append({"range": [m.start(), m.end()], "kind": "trailing-space",
                            "message": "remover espaço ao fim da linha"})
    for m in SENTENCE_START.finditer(text):
        idx = m.start(1)
        suggestions.append({"range": [idx, idx + 1], "kind": "capitalize",
                            "message": f"começar com maiúscula '{m.group(1)}'"})

    payload = {
        "type": "format",
        "docId": doc_id,
        "atSeq": job.get("atSeq", 0),
        "suggestions": suggestions,
        "worker": os.environ.get("HOSTNAME", "formatter"),
    }
    await _pub.publish(f"doc:{doc_id}:annotations", json.dumps(payload))
    log.info("doc=%s atSeq=%s suggestions=%d", doc_id, job.get("atSeq"), len(suggestions))


if __name__ == "__main__":
    asyncio.run(run_consumer(JOBS_FORMAT, GROUP_FORMATTERS, handle))
