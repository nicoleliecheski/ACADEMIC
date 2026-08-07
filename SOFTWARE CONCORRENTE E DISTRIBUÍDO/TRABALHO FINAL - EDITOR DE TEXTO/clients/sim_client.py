from __future__ import annotations
import argparse
import asyncio
import json
import os
import random
import string
import sys
import urllib.request
import websockets

GW_HTTP = os.environ.get("GW_HTTP", "http://localhost:8080")
GW_WS = os.environ.get("GW_WS", "ws://localhost:8081")

WORDS = ["alfa", "beta", "gama", "delta", "dados", "texto", "editar", "linha", "mesclar", "rápido", "cliente", "servidor", "documento", "mundo", "olá", "nó"]


def apply_op(text: str, op: dict) -> str:
    pos = max(0, min(int(op["pos"]), len(text)))
    if op["kind"] == "insert":
        return text[:pos] + op.get("text", "") + text[pos:]
    end = min(pos + max(0, int(op.get("len", 0))), len(text))
    return text[:pos] + text[end:]


def rest(method: str, path: str, body: dict | None = None) -> dict:
    url = f"{GW_HTTP}{path}"
    data = json.dumps(body).encode() if body is not None else None
    req = urllib.request.Request(url, data=data, method=method,
                                 headers={"content-type": "application/json"} if data else {})
    with urllib.request.urlopen(req, timeout=5) as resp:
        return json.loads(resp.read().decode())


class Editor:

    def __init__(self, doc_id: str, client_id: str):
        self.doc_id = doc_id
        self.client_id = client_id
        self.text = ""
        self.seq = 0
        self.ws = None
        self._recv_task = None
        self._ready = asyncio.Event()

    async def connect(self):
        self.ws = await websockets.connect(GW_WS, max_size=4 * 1024 * 1024)
        await self.ws.send(json.dumps({"type": "join", "docId": self.doc_id,
                                       "clientId": self.client_id}))
        self._recv_task = asyncio.create_task(self._recv_loop())
        await self._ready.wait()

    async def _recv_loop(self):
        async for raw in self.ws:
            msg = json.loads(raw)
            t = msg.get("type")
            if t == "resync":
                self.text = msg["text"]
                self.seq = msg["seq"]
                self._ready.set()
            elif t == "op.applied":
                seq = msg["seq"]
                if seq == self.seq + 1:
                    self.text = apply_op(self.text, msg["transformedOp"])
                    self.seq = seq
                elif seq > self.seq + 1:
                    await self._resync_rest()
            elif t == "annotation":
                pass 

    async def _resync_rest(self):
        doc = await asyncio.to_thread(rest, "GET", f"/docs/{self.doc_id}")
        self.text = doc["text"]
        self.seq = doc["seq"]

    async def send_op(self, op: dict, op_seq: int):
        await self.ws.send(json.dumps({
            "type": "op", "docId": self.doc_id, "clientId": self.client_id,
            "baseVersion": self.seq, "op": op, "opId": f"{self.client_id}:{op_seq}",
        }))

    async def close(self):
        if self.ws:
            await self.ws.close()
        if self._recv_task:
            self._recv_task.cancel()


def random_op(text: str) -> dict:
    if text and random.random() < 0.25:
        pos = random.randint(0, len(text) - 1)
        return {"kind": "delete", "pos": pos, "len": 1}
    pos = random.randint(0, len(text))
    return {"kind": "insert", "pos": pos, "text": random.choice(WORDS) + " "}


async def cmd_create(args):
    out = rest("POST", "/docs", {"docId": args.doc, "initialText": args.text or ""})
    print(f"criado {out['docId']} no {out['shardId']} seq={out['seq']}")


async def cmd_type(args):
    ed = Editor(args.doc, args.client)
    await ed.connect()
    for i, ch in enumerate(args.text):
        await ed.send_op({"kind": "insert", "pos": len(ed.text), "text": ch}, i + 1)
        await asyncio.sleep(args.delay)
    await asyncio.sleep(1.0)
    print(f"[{args.client}] digitou; seq local={ed.seq} texto={ed.text!r}")
    await ed.close()


async def cmd_edit(args):
    ed = Editor(args.doc, args.client)
    await ed.connect()
    for i in range(args.ops):
        await ed.send_op(random_op(ed.text), i + 1)
        await asyncio.sleep(args.delay)
    await asyncio.sleep(1.0)
    print(f"[{args.client}] enviou {args.ops} ops; seq local={ed.seq} tam={len(ed.text)}")
    await ed.close()


async def _run_editor(doc_id, client_id, n_ops, delay):
    ed = Editor(doc_id, client_id)
    await ed.connect()
    for i in range(n_ops):
        await ed.send_op(random_op(ed.text), i + 1)
        await asyncio.sleep(delay + random.random() * delay)
    return ed


async def cmd_converge(args):
    editors = await asyncio.gather(*[
        _run_editor(args.doc, f"c{i+1}", args.ops, args.delay) for i in range(args.clients)
    ])
    print(f"todos os {args.clients} clientes terminaram de enviar; drenando eventos...")
    await asyncio.sleep(3.0)

    server = await asyncio.to_thread(rest, "GET", f"/docs/{args.doc}")
    print(f"servidor: seq={server['seq']} tam={len(server['text'])} servidoPor={server.get('servedBy')}")

    ok = True
    for ed in editors:
        match = (ed.text == server["text"] and ed.seq == server["seq"])
        ok = ok and match
        print(f"  [{ed.client_id}] seq={ed.seq} tam={len(ed.text)} "
              f"convergiu={'SIM' if match else 'NÃO'}")
        await ed.close()

    print("CONVERGÊNCIA:", "PASSOU ✅" if ok else "FALHOU ❌")
    if not ok:
        sys.exit(1)


async def cmd_get(args):
    path = f"/docs/{args.doc}" + ("?replica=1" if args.replica else "")
    out = rest("GET", path)
    print(json.dumps(out, indent=2, ensure_ascii=False))


async def cmd_watch(args):
    ed = Editor(args.doc, args.client)
    ed.ws = await websockets.connect(GW_WS, max_size=4 * 1024 * 1024)
    await ed.ws.send(json.dumps({"type": "join", "docId": args.doc, "clientId": args.client}))
    print(f"[{args.client}] observando {args.doc} por {args.seconds}s ...")

    async def printer():
        async for raw in ed.ws:
            msg = json.loads(raw)
            if msg.get("type") == "annotation":
                inner = msg.get("issues") or msg.get("suggestions") or []
                print(f"  anotação[{msg.get('worker')}] atSeq={msg.get('atSeq')} "
                      f"itens={len(inner)}: {json.dumps(inner, ensure_ascii=False)[:200]}")
            else:
                print(f"  {msg.get('type')}: {json.dumps(msg, ensure_ascii=False)[:160]}")

    try:
        await asyncio.wait_for(printer(), timeout=args.seconds)
    except asyncio.TimeoutError:
        pass
    await ed.ws.close()


def main():
    p = argparse.ArgumentParser(description="Cliente simulado do editor colaborativo")
    sub = p.add_subparsers(dest="cmd", required=True)

    sc = sub.add_parser("create"); sc.add_argument("--doc", required=True); sc.add_argument("--text", default="")
    st = sub.add_parser("type"); st.add_argument("--doc", required=True); st.add_argument("--client", default="digitador")
    st.add_argument("--text", required=True); st.add_argument("--delay", type=float, default=0.05)
    se = sub.add_parser("edit"); se.add_argument("--doc", required=True); se.add_argument("--client", default="c1")
    se.add_argument("--ops", type=int, default=20); se.add_argument("--delay", type=float, default=0.05)
    cv = sub.add_parser("converge"); cv.add_argument("--doc", required=True)
    cv.add_argument("--clients", type=int, default=5); cv.add_argument("--ops", type=int, default=10)
    cv.add_argument("--delay", type=float, default=0.03)
    sg = sub.add_parser("get"); sg.add_argument("--doc", required=True); sg.add_argument("--replica", action="store_true")
    sw = sub.add_parser("watch"); sw.add_argument("--doc", required=True); sw.add_argument("--client", default="observador")
    sw.add_argument("--seconds", type=float, default=10)

    args = p.parse_args()
    fn = {
        "create": cmd_create, "type": cmd_type, "edit": cmd_edit,
        "converge": cmd_converge, "get": cmd_get, "watch": cmd_watch,
    }[args.cmd]
    asyncio.run(fn(args))


if __name__ == "__main__":
    main()
