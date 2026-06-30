// Shard router: maps a docId to its shard (consistent-hash ring) and resolves
// the shard's current PRIMARY from the Redis lease key. Results are cached and
// invalidated on `primary.changed` cluster events or on RPC failure.

import crypto from "node:crypto";
import { SHARDMAP, leaseKey } from "./names.js";

function hash32(s) {
  // Stable 32-bit hash (md5 prefix) — deterministic across Node and Python.
  const h = crypto.createHash("md5").update(s).digest();
  return h.readUInt32BE(0);
}

export class Router {
  constructor(redis) {
    this.redis = redis;
    this.ring = [];        // sorted [{ point, shardId }] virtual nodes
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

  // Consistent-hash lookup: first ring point clockwise from hash(docId).
  shardForDoc(docId) {
    if (this.ring.length === 0) throw new Error("router not loaded");
    const hp = hash32(docId);
    for (const node of this.ring) {
      if (node.point >= hp) return node.shardId;
    }
    return this.ring[0].shardId; // wrap around
  }

  shardConfig(shardId) {
    return this.shards.find((s) => s.shardId === shardId);
  }

  invalidatePrimary(shardId) {
    this.primaryCache.delete(shardId);
  }

  // Resolve the live primary address by reading the lease key.
  async primaryAddr(shardId) {
    const cached = this.primaryCache.get(shardId);
    if (cached) return cached;
    const addr = await this.redis.get(leaseKey(shardId));
    if (!addr) throw new Error(`no primary holds the lease for ${shardId}`);
    this.primaryCache.set(shardId, addr);
    return addr;
  }

  // A replica address (for read-from-replica): any configured node that is not
  // the current primary; fall back to the primary if none.
  async replicaAddr(shardId) {
    const cfg = this.shardConfig(shardId);
    if (!cfg) throw new Error(`unknown shard ${shardId}`);
    let primary = null;
    try { primary = await this.primaryAddr(shardId); } catch { /* ignore */ }
    const candidates = cfg.nodes.filter((a) => a !== primary);
    return candidates.length ? candidates[0] : primary;
  }
}
