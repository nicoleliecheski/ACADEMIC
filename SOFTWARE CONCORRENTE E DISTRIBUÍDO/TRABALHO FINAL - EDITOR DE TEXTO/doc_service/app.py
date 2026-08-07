from __future__ import annotations
import asyncio
import json
import logging
import time
from contextlib import asynccontextmanager
from typing import Any, Dict
import redis.asyncio as aioredis
from fastapi import FastAPI, HTTPException
from fastapi.responses import JSONResponse
from pydantic import BaseModel
from config import config
from jobs import JobEnqueuer
from lease import LeaseManager
from names import doc_channel, snapshot_key
from ops import rebase
from replication import Replicator
from store import DocStore

logging.basicConfig(
    level=logging.INFO,
    format=f"%(asctime)s [{config.SHARD_ID}/{config.NODE_ID}] %(name)s %(levelname)s %(message)s",
)
log = logging.getLogger("doc_service")


class Node:

    def __init__(self) -> None:
        self.redis: aioredis.Redis = aioredis.from_url(config.REDIS_URL, decode_responses=True)
        self.store = DocStore(config.SHARD_ID)
        self.replicator = Replicator(self.redis, self.store)
        self.jobs = JobEnqueuer(self.redis, self.store)
        self.lease = LeaseManager(self.redis, self.on_role_change)

    @property
    def is_primary(self) -> bool:
        return self.lease.is_primary

    async def on_role_change(self, primary: bool) -> None:
        if primary:
            await self.replicator.drain()
            await self.replicator.stop_consuming()
            self.jobs.start()
            log.info("now PRIMARY: head seqs=%s",
                     {d: self.store.get(d).seq for d in self.store.all_doc_ids()})
        else:
            await self.jobs.stop()
            self.replicator.start_consuming()

    async def startup(self) -> None:
        await self.redis.ping()
        self.replicator.start_consuming()
        await self.lease.start()
        log.info("started node addr=%s preferred=%s", config.advertise_addr, config.PREFERRED_ROLE)

    async def shutdown(self) -> None:
        await self.lease.stop()
        await self.jobs.stop()
        await self.replicator.stop_consuming()
        await self.redis.aclose()


node: Node 


@asynccontextmanager
async def lifespan(app: FastAPI):
    global node
    node = Node()
    await node.startup()
    try:
        yield
    finally:
        await node.shutdown()


app = FastAPI(title="doc-service", lifespan=lifespan)


class CreateReq(BaseModel):
    docId: str
    initialText: str = ""


class OpReq(BaseModel):
    docId: str
    clientId: str
    baseVersion: int
    op: Dict[str, Any]
    opId: str


@app.get("/health")
async def health():
    return {"ok": True, "shardId": config.SHARD_ID, "nodeId": config.NODE_ID}


@app.get("/role")
async def role():
    return {
        "nodeId": config.NODE_ID,
        "shardId": config.SHARD_ID,
        "addr": config.advertise_addr,
        "role": "primary" if node.is_primary else "replica",
        "docs": {d: node.store.get(d).seq for d in node.store.all_doc_ids()},
    }


def _require_primary():
    if not node.is_primary:
        raise HTTPException(status_code=409, detail="not primary for this shard")


@app.post("/rpc/create")
async def rpc_create(req: CreateReq):
    _require_primary()
    doc = await node.store.get_or_create(req.docId)
    async with doc.lock:
        if req.initialText and doc.seq == 0 and not doc.text:
            doc.snapshot_text = req.initialText
            doc.text = req.initialText
        await node.replicator.produce({
            "type": "create", "docId": req.docId, "shardId": config.SHARD_ID,
            "text": doc.text, "ts": time.time(),
        })
        await node.redis.set(snapshot_key(req.docId), json.dumps({
            "docId": req.docId, "baseVersion": doc.base_version,
            "text": doc.snapshot_text, "ts": time.time(),
        }))
    return doc.summary()


@app.post("/rpc/op")
async def rpc_op(req: OpReq):
    _require_primary()
    doc = await node.store.get_or_create(req.docId)
    async with doc.lock:
        if req.opId in doc.seen_op_ids:
            for e in reversed(doc.oplog):
                if e.get("opId") == req.opId:
                    return {"seq": e["seq"], "transformedOp": e["op"], "duplicate": True}
            return {"seq": doc.seq, "transformedOp": req.op, "duplicate": True}

        intervening = doc.intervening_ops(req.baseVersion)
        transformed = rebase(req.op, intervening)

        seq = doc.seq + 1
        entry = {
            "type": "op", "seq": seq, "docId": req.docId, "shardId": config.SHARD_ID,
            "clientId": req.clientId, "opId": req.opId,
            "op": transformed, "ts": time.time(), "appliedBy": config.NODE_ID,
        }
        doc.append_applied(entry, config.SNAPSHOT_EVERY)
        await node.replicator.produce(entry)
        node.jobs.mark_dirty(req.docId, seq)
        await node.redis.publish(doc_channel(req.docId), json.dumps({
            "type": "op.applied", "docId": req.docId, "seq": seq,
            "clientId": req.clientId, "opId": req.opId, "transformedOp": transformed,
        }))
    return {"seq": seq, "transformedOp": transformed, "duplicate": False}


@app.post("/rpc/snapshot")
async def rpc_snapshot(req: CreateReq):
    _require_primary()
    doc = node.store.get(req.docId)
    if doc is None:
        raise HTTPException(status_code=404, detail="no such document")
    async with doc.lock:
        doc.compact()
        await node.redis.set(snapshot_key(req.docId), json.dumps({
            "docId": req.docId, "baseVersion": doc.base_version,
            "text": doc.snapshot_text, "ts": time.time(),
        }))
    return {"docId": req.docId, "baseVersion": doc.base_version, "seq": doc.seq}


@app.get("/rpc/doc/{doc_id}")
async def rpc_doc(doc_id: str):
    doc = node.store.get(doc_id)
    if doc is None:
        raise HTTPException(status_code=404, detail="no such document")
    out = doc.summary()
    out["servedBy"] = "primary" if node.is_primary else "replica"
    out["nodeId"] = config.NODE_ID
    return out


@app.get("/rpc/ops/{doc_id}")
async def rpc_ops(doc_id: str, since: int = 0):
    doc = node.store.get(doc_id)
    if doc is None:
        raise HTTPException(status_code=404, detail="no such document")
    return doc.ops_since(since)


@app.exception_handler(HTTPException)
async def _http_exc(_request, exc: HTTPException):
    return JSONResponse(status_code=exc.status_code,
                        content={"error": exc.detail, "shardId": config.SHARD_ID})
