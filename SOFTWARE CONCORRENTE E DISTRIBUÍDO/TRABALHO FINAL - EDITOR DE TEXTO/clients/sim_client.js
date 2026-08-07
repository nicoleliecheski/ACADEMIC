import WebSocket from "ws";

const GW_WS = process.env.GW_WS || "ws://localhost:8081";
const WORDS = ["alfa", "beta", "gama", "nó", "editar", "sync", "texto", "vivo"];

function arg(name, def) {
  const i = process.argv.indexOf(`--${name}`);
  return i >= 0 ? process.argv[i + 1] : def;
}

function applyOp(text, op) {
  const pos = Math.max(0, Math.min(op.pos, text.length));
  if (op.kind === "insert") return text.slice(0, pos) + (op.text || "") + text.slice(pos);
  const end = Math.min(pos + (op.len || 0), text.length);
  return text.slice(0, pos) + text.slice(end);
}

const cmd = process.argv[2];
const docId = arg("doc", "doc-A");
const clientId = arg("client", "n1");
const ops = parseInt(arg("ops", "15"), 10);
const literal = arg("text", "ola mundo");
const delay = parseFloat(arg("delay", "0.05")) * 1000;

const state = { text: "", seq: 0 };
const ws = new WebSocket(GW_WS);

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

ws.on("open", () => {
  ws.send(JSON.stringify({ type: "join", docId, clientId }));
});

ws.on("message", async (raw) => {
  const msg = JSON.parse(raw.toString());
  if (msg.type === "resync") {
    state.text = msg.text;
    state.seq = msg.seq;
    await drive();
  } else if (msg.type === "op.applied") {
    if (msg.seq === state.seq + 1) {
      state.text = applyOp(state.text, msg.transformedOp);
      state.seq = msg.seq;
    }
  }
});

async function drive() {
  if (cmd === "type") {
    for (const ch of literal) {
      ws.send(JSON.stringify({
        type: "op", docId, clientId, baseVersion: state.seq,
        op: { kind: "insert", pos: state.text.length, text: ch }, opId: `${clientId}:${state.seq + 1}`,
      }));
      await sleep(delay);
    }
  } else {
    for (let i = 0; i < ops; i++) {
      const pos = Math.floor(Math.random() * (state.text.length + 1));
      const op = { kind: "insert", pos, text: WORDS[i % WORDS.length] + " " };
      ws.send(JSON.stringify({
        type: "op", docId, clientId, baseVersion: state.seq, op, opId: `${clientId}:${i + 1}`,
      }));
      await sleep(delay);
    }
  }
  await sleep(1000);
  console.log(`[${clientId}] concluído; seq local=${state.seq} tam=${state.text.length}`);
  ws.close();
  process.exit(0);
}
