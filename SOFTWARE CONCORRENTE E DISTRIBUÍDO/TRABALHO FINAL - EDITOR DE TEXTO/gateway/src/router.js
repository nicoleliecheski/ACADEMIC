// Roteador de shards: mapeia um docId para o seu shard (anel de hash consistente)
// e resolve o PRIMÁRIO atual do shard a partir da chave de lease no Redis. Os
// resultados são cacheados e invalidados em eventos `primary.changed` do cluster
// ou em falha de RPC.

import crypto from "node:crypto";
import { SHARDMAP, leaseKey } from "./names.js";

function hash32(s) {
  // Hash estável de 32 bits (prefixo md5), determinístico entre Node e Python.
  const h = crypto.createHash("md5").update(s).digest();
  return h.readUInt32BE(0);
}

export class Router {
  constructor(redis) {
    this.redis = redis;
    this.ring = [];        // [{ point, shardId }] ordenado (nós virtuais)
    this.shards = [];      // [{ shardId, replicas: [addr,...] }]
    this.primaryCache = new Map(); // shardId -> addr
  }

  async loadShardMap() {
    const raw = await this.redis.get(SHARDMAP);
    if (!raw) throw new Error("shardmap not initialized in Redis");
    const map = JSON.parse(raw);
    this.shards = map.shards;
    const vnodes = map.vnodes || 64;
    const ring = [];
    for (const shard of this.shards) {
      for (let i = 0; i < vnodes; i++) {
        ring.push({ point: hash32(`${shard.shardId}#${i}`), shardId: shard.shardId });
      }
    }
    ring.sort((a, b) => a.point - b.point);
    this.ring = ring;
    return map;
  }

  // Busca por hash consistente: primeiro ponto do anel no sentido horário a
  // partir de hash(docId).
  shardForDoc(docId) {
    if (this.ring.length === 0) throw new Error("router not loaded");
    const hp = hash32(docId);
    for (const node of this.ring) {
      if (node.point >= hp) return node.shardId;
    }
    return this.ring[0].shardId; // dá a volta
  }

  shardConfig(shardId) {
    return this.shards.find((s) => s.shardId === shardId);
  }

  invalidatePrimary(shardId) {
    this.primaryCache.delete(shardId);
  }

  // Resolve o endereço do primário ativo lendo a chave de lease.
  async primaryAddr(shardId) {
    const cached = this.primaryCache.get(shardId);
    if (cached) return cached;
    const addr = await this.redis.get(leaseKey(shardId));
    if (!addr) throw new Error(`no primary holds the lease for ${shardId}`);
    this.primaryCache.set(shardId, addr);
    return addr;
  }

  // Um endereço de réplica (para leitura na réplica): qualquer nó configurado
  // que não seja o primário atual; cai para o primário se não houver outro.
  async replicaAddr(shardId) {
    const cfg = this.shardConfig(shardId);
    if (!cfg) throw new Error(`unknown shard ${shardId}`);
    let primary = null;
    try { primary = await this.primaryAddr(shardId); } catch { /* ignora */ }
    const candidates = cfg.nodes.filter((a) => a !== primary);
    return candidates.length ? candidates[0] : primary;
  }
}
