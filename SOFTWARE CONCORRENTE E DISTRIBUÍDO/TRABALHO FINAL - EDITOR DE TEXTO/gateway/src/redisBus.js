// Ponte de pub/sub do Redis (o caminho de notificação ASSÍNCRONO). Uma conexão
// dedicada de assinante faz pattern-subscribe em todos os canais de documento e
// escuta os eventos de cluster; mensagens decodificadas são despachadas para os
// handlers registrados.

import { CLUSTER_EVENTS } from "./names.js";

export class RedisBus {
  constructor(subRedis) {
    this.sub = subRedis;
    this.onDocEvent = null;      // (docId, kind, payload) => void
    this.onClusterEvent = null;  // (payload) => void
  }

  async start() {
    // doc:*  casa com doc:{id}, doc:{id}:annotations, doc:{id}:presence
    await this.sub.psubscribe("doc:*");
    await this.sub.subscribe(CLUSTER_EVENTS);

    this.sub.on("pmessage", (_pattern, channel, message) => {
      let payload;
      try { payload = JSON.parse(message); } catch { return; }
      const parts = channel.split(":"); // ["doc", "{id}", talvez "annotations"]
      const docId = parts[1];
      const kind = parts[2] || "op"; // "op" | "annotations" | "presence"
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
