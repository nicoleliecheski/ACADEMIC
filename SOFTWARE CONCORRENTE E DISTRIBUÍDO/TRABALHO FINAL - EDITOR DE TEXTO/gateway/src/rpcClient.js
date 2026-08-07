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
        this.router.invalidatePrimary(shardId);
        await new Promise((r) => setTimeout(r, Math.min(700, 250 * (attempt + 1))));
      }
    }
    throw lastErr;
  }
}
