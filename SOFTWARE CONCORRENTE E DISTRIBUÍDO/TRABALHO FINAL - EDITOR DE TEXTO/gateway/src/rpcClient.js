// Cliente HTTP/JSON enxuto para o RPC do doc-service -- este é o caminho de
// interação SÍNCRONO (bloqueante). As chamadas de escrita re-resolvem o primário
// e tentam novamente automaticamente em caso de 409 ("not primary"); é assim que
// o gateway atravessa de forma transparente a troca de réplica (failover).

const DEFAULT_TIMEOUT_MS = 4000;

async function httpJson(method, url, body, timeoutMs = DEFAULT_TIMEOUT_MS) {
  const ctrl = new AbortController();
  const timer = setTimeout(() => ctrl.abort(), timeoutMs);
  try {
    const res = await fetch(url, {
      method,
      headers: body ? { "content-type": "application/json" } : undefined,
      body: body ? JSON.stringify(body) : undefined,
      signal: ctrl.signal,
    });
    const text = await res.text();
    const data = text ? JSON.parse(text) : {};
    if (!res.ok) {
      const err = new Error(data.error || `HTTP ${res.status}`);
      err.status = res.status;
      throw err;
    }
    return data;
  } finally {
    clearTimeout(timer);
  }
}

export class RpcClient {
  constructor(router) {
    this.router = router;
  }

  async read(docId, { fromReplica = false } = {}) {
    const shardId = this.router.shardForDoc(docId);
    const addr = fromReplica
      ? await this.router.replicaAddr(shardId)
      : await this.router.primaryAddr(shardId);
    return httpJson("GET", `http://${addr}/rpc/doc/${encodeURIComponent(docId)}`);
  }

  async opsSince(docId, since) {
    const shardId = this.router.shardForDoc(docId);
    const addr = await this.router.primaryAddr(shardId);
    return httpJson("GET", `http://${addr}/rpc/ops/${encodeURIComponent(docId)}?since=${since}`);
  }

  // Escrita com re-resolução automática do primário + retry (disponibilidade).
  // O orçamento de retry (~12 x até 700ms ≈ 8s) é propositalmente maior que o
  // pior caso de tomada do lease (TTL + um intervalo de renovação da réplica),
  // então uma escrita em andamento quando o primário cai atravessa o failover e
  // tem sucesso.
  async write(path, docId, body, { retries = 12 } = {}) {
    const shardId = this.router.shardForDoc(docId);
    let lastErr;
    for (let attempt = 0; attempt <= retries; attempt++) {
      let addr;
      try {
        addr = await this.router.primaryAddr(shardId);
        return await httpJson("POST", `http://${addr}/rpc/${path}`, body);
      } catch (err) {
        lastErr = err;
        // 409 = primário desatualizado; erro de rede = primário provavelmente
        // caiu. Em ambos os casos, descarta o cache, espera o lease estabilizar
        // e tenta de novo.
        this.router.invalidatePrimary(shardId);
        await new Promise((r) => setTimeout(r, Math.min(700, 250 * (attempt + 1))));
      }
    }
    throw lastErr;
  }
}
