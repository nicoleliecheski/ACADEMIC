// Serviço de gateway / borda.
//   * API REST (síncrona, bloqueante) na HTTP_PORT -> create/open/ops/snapshot/save
//   * WebSocket (assíncrono, ao vivo)  na WS_PORT   -> edição ao vivo + notificações
//   * ponte de pub/sub do Redis        -> repassa op.applied / annotations aos clientes WS
//   * roteador de shards               -> particionamento + resolução do primário / failover
//
// Duas linguagens se encontram aqui: esta borda em Node fala HTTP/JSON com os
// nós do serviço de documentos em Python e compartilha o Redis com os workers
// em Python.

import http from "node:http";
import path from "node:path";
import crypto from "node:crypto";
import { fileURLToPath } from "node:url";
import express from "express";
import Redis from "ioredis";

import { Router } from "./router.js";
import { RpcClient } from "./rpcClient.js";
import { RedisBus } from "./redisBus.js";
import { WsHub } from "./wsHub.js";
import { SHARDMAP } from "./names.js";

const HTTP_PORT = parseInt(process.env.HTTP_PORT || "8080", 10);
const WS_PORT = parseInt(process.env.WS_PORT || "8081", 10);
const REDIS_URL = process.env.REDIS_URL || "redis://localhost:6379/0";
const WEBUI_DIR = process.env.WEBUI_DIR ||
  path.resolve(fileURLToPath(new URL("../../webui", import.meta.url)));

const log = (...a) => console.log(new Date().toISOString(), ...a);

// --- Conexões Redis (comando, publicação e assinatura devem ser separadas) --
const redis = new Redis(REDIS_URL);
const pub = new Redis(REDIS_URL);
const sub = new Redis(REDIS_URL);

const router = new Router(redis);
const rpc = new RpcClient(router);

// Semeia um mapa de shards padrão no primeiro boot, se nenhum existir.
// GW_SHARDMAP é uma string JSON descrevendo os shards e os endereços dos seus
// nós (veja o docker-compose).
async function ensureShardMap() {
  const existing = await redis.get(SHARDMAP);
  if (existing) return;
  const seed = process.env.GW_SHARDMAP;
  if (!seed) throw new Error("no shardmap in Redis and GW_SHARDMAP not set");
  await redis.set(SHARDMAP, seed);
  log("[BOOT] seeded shardmap from GW_SHARDMAP");
}

// --------------------------------------------------------------------------- //
// API REST (caminho síncrono / bloqueante)
// --------------------------------------------------------------------------- //
const app = express();
app.use(express.json());
app.use(express.static(WEBUI_DIR));

app.get("/health", (_req, res) => res.json({ ok: true }));

app.get("/shardmap", async (_req, res) => {
  res.json({ shards: router.shards, ring: router.ring.length });
});

// Visão agregada do cluster: quem é primário/réplica por shard neste momento.
app.get("/cluster", async (_req, res) => {
  const out = [];
  for (const shard of router.shards) {
    const nodes = [];
    for (const addr of shard.nodes) {
      try {
        const r = await fetch(`http://${addr}/role`, { signal: AbortSignal.timeout(1500) });
        nodes.push(await r.json());
      } catch (e) {
        nodes.push({ addr, role: "down", error: String(e) });
      }
    }
    out.push({ shardId: shard.shardId, nodes });
  }
  res.json({ shards: out });
});

app.post("/docs", async (req, res) => {
  const docId = req.body.docId || `doc-${crypto.randomUUID().slice(0, 8)}`;
  const initialText = req.body.initialText || "";
  log(`[SYNC] POST /docs id=${docId} -> shard ${router.shardForDoc(docId)}`);
  try {
    const out = await rpc.write("create", docId, { docId, initialText });
    res.json(out);
  } catch (err) {
    res.status(502).json({ error: err.message });
  }
});

app.get("/docs/:id", async (req, res) => {
  const fromReplica = req.query.replica === "1" || req.query.read_from_replica === "true";
  log(`[SYNC] GET /docs/${req.params.id} replica=${fromReplica}`);
  try {
    res.json(await rpc.read(req.params.id, { fromReplica }));
  } catch (err) {
    res.status(404).json({ error: err.message });
  }
});

app.get("/docs/:id/ops", async (req, res) => {
  const since = parseInt(req.query.since || "0", 10);
  log(`[SYNC] GET /docs/${req.params.id}/ops?since=${since}`);
  try {
    res.json(await rpc.opsSince(req.params.id, since));
  } catch (err) {
    res.status(404).json({ error: err.message });
  }
});

app.post("/docs/:id/snapshot", async (req, res) => {
  log(`[SYNC] POST /docs/${req.params.id}/snapshot`);
  try {
    res.json(await rpc.write("snapshot", req.params.id, { docId: req.params.id }));
  } catch (err) {
    res.status(502).json({ error: err.message });
  }
});

// "save" é um alias voltado ao cliente que força um snapshot durável.
app.post("/docs/:id/save", async (req, res) => {
  log(`[SYNC] POST /docs/${req.params.id}/save`);
  try {
    res.json(await rpc.write("snapshot", req.params.id, { docId: req.params.id }));
  } catch (err) {
    res.status(502).json({ error: err.message });
  }
});

// --------------------------------------------------------------------------- //
// Servidor WebSocket (caminho assíncrono / ao vivo)
// --------------------------------------------------------------------------- //
const wsServer = http.createServer();
const hub = new WsHub({ server: wsServer, rpc, pubRedis: pub, log });

// --------------------------------------------------------------------------- //
// Bus do Redis -> difusão para o WS
// --------------------------------------------------------------------------- //
const bus = new RedisBus(sub);
bus.onDocEvent = (docId, kind, payload) => {
  if (kind === "annotations") hub.broadcast(docId, { ...payload, type: "annotation" });
  else hub.broadcast(docId, payload); // op.applied / presence passam direto
};
bus.onClusterEvent = (payload) => {
  if (payload.type === "primary.changed") {
    router.invalidatePrimary(payload.shardId);
    log(`[CLUSTER] primary.changed shard=${payload.shardId} -> ${payload.primary}`);
  }
};

// --------------------------------------------------------------------------- //
// Boot
// --------------------------------------------------------------------------- //
async function main() {
  await ensureShardMap();
  await router.loadShardMap();
  await bus.start();
  app.listen(HTTP_PORT, () => log(`[BOOT] REST + UI on :${HTTP_PORT}`));
  wsServer.listen(WS_PORT, () => log(`[BOOT] WebSocket on :${WS_PORT}`));
}

main().catch((err) => {
  console.error("gateway failed to start:", err);
  process.exit(1);
});
