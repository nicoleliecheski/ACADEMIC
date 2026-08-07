import { CLUSTER_EVENTS } from "./names.js";

export class RedisBus {
  constructor(subRedis) {
    this.sub = subRedis;
    this.onDocEvent = null;      
    this.onClusterEvent = null; 
  }

  async start() {
    await this.sub.psubscribe("doc:*");
    await this.sub.subscribe(CLUSTER_EVENTS);

    this.sub.on("pmessage", (_pattern, channel, message) => {
      let payload;
      try { payload = JSON.parse(message); } catch { return; }
      const parts = channel.split(":");
      const docId = parts[1];
      const kind = parts[2] || "op"; 
      if (this.onDocEvent) this.onDocEvent(docId, kind, payload);
    });

    this.sub.on("message", (channel, message) => {
      if (channel !== CLUSTER_EVENTS) return;
      let payload;
      try { payload = JSON.parse(message); } catch { return; }
      if (this.onClusterEvent) this.onClusterEvent(payload);
    });
  }
}
