import { WebSocketServer } from "ws";
import { presenceChannel } from "./names.js";

export class WsHub {
  constructor({ server, rpc, pubRedis, log }) {
    this.rooms = new Map();        
    this.rpc = rpc;
    this.pub = pubRedis;
    this.log = log;
    this.wss = new WebSocketServer({ server });
    this.wss.on("connection", (ws) => this._onConnect(ws));
  }

  _onConnect(ws) {
    ws.docId = null;
    ws.clientId = null;
    ws.isAlive = true;
    ws.on("message", (data) => this._onMessage(ws, data));
    ws.on("close", () => this._leave(ws));
    ws.on("error", () => this._leave(ws));
  }

  async _onMessage(ws, data) {
    let msg;
    try { msg = JSON.parse(data.toString()); } catch { return; }

    switch (msg.type) {
      case "join":
        return this._join(ws, msg);
      case "op":
        return this._op(ws, msg);
      case "cursor":
        return this._cursor(ws, msg);
      case "ping":
        return this._send(ws, { type: "pong", t: msg.t });
      default:
        return this._send(ws, { type: "error", error: `unknown type ${msg.type}` });
    }
  }

  async _join(ws, msg) {
    ws.docId = msg.docId;
    ws.clientId = msg.clientId;
    if (!this.rooms.has(msg.docId)) this.rooms.set(msg.docId, new Set());
    this.rooms.get(msg.docId).add(ws);
    this.log(`WS join doc=${msg.docId} client=${msg.clientId}`);

    try {
      let doc;
      try {
        doc = await this.rpc.read(msg.docId);
      } catch (readErr) {
        await this.rpc.write("create", msg.docId, { docId: msg.docId, initialText: "" });
        doc = await this.rpc.read(msg.docId);
      }
      this._send(ws, { type: "resync", docId: doc.docId, seq: doc.seq, text: doc.text });
    } catch (err) {
      this._send(ws, { type: "error", error: `join failed: ${err.message}` });
    }
    this._publishPresence(msg.docId, msg.clientId, "joined");
  }

  async _op(ws, msg) {
    try {
      await this.rpc.write("op", msg.docId, {
        docId: msg.docId,
        clientId: msg.clientId,
        baseVersion: msg.baseVersion,
        op: msg.op,
        opId: msg.opId,
      });
    } catch (err) {
      this._send(ws, { type: "error", error: `op rejected: ${err.message}`, opId: msg.opId });
    }
  }

  _cursor(ws, msg) {
    this._publishPresence(msg.docId, msg.clientId, "cursor", { pos: msg.pos });
  }

  _publishPresence(docId, clientId, event, extra = {}) {
    const payload = JSON.stringify({ type: "presence", docId, clientId, event, ...extra });
    this.pub.publish(presenceChannel(docId), payload).catch(() => {});
  }

  _leave(ws) {
    if (ws.docId && this.rooms.has(ws.docId)) {
      this.rooms.get(ws.docId).delete(ws);
      if (ws.clientId) this._publishPresence(ws.docId, ws.clientId, "left");
    }
  }

  broadcast(docId, obj) {
    const room = this.rooms.get(docId);
    if (!room) return;
    const data = JSON.stringify(obj);
    for (const ws of room) {
      if (ws.readyState === ws.OPEN) ws.send(data);
    }
  }

  _send(ws, obj) {
    if (ws.readyState === ws.OPEN) ws.send(JSON.stringify(obj));
  }
}
