const $ = (id) => document.getElementById(id);
const REST = location.origin;                         
const WS_URL = `ws://${location.hostname}:8081`;

let ws = null;
let docId = null;
let clientId = null;
let serverSeq = 0;
let lastText = "";          
let opCounter = 0;
let applyingRemote = false; 
const presence = new Map();

function setStatus(connected) {
  const el = $("status");
  el.textContent = connected ? "conectado" : "desconectado";
  el.className = "status " + (connected ? "connected" : "disconnected");
}

function applyOp(text, op) {
  const pos = Math.max(0, Math.min(op.pos, text.length));
  if (op.kind === "insert") return text.slice(0, pos) + (op.text || "") + text.slice(pos);
  const end = Math.min(pos + (op.len || 0), text.length);
  return text.slice(0, pos) + text.slice(end);
}

function diffToOp(oldText, newText) {
  if (oldText === newText) return null;
  let start = 0;
  const minLen = Math.min(oldText.length, newText.length);
  while (start < minLen && oldText[start] === newText[start]) start++;
  let endOld = oldText.length, endNew = newText.length;
  while (endOld > start && endNew > start && oldText[endOld - 1] === newText[endNew - 1]) {
    endOld--; endNew--;
  }
  const removed = oldText.slice(start, endOld);
  const added = newText.slice(start, endNew);
  if (added && !removed) return { kind: "insert", pos: start, text: added };
  if (removed && !added) return { kind: "delete", pos: start, len: removed.length };
  return [{ kind: "delete", pos: start, len: removed.length }, { kind: "insert", pos: start, text: added }];
}

function sendOp(op) {
  opCounter++;
  ws.send(JSON.stringify({
    type: "op", docId, clientId, baseVersion: serverSeq, op,
    opId: `${clientId}:${opCounter}`,
  }));
}

function onEditorInput() {
  if (applyingRemote) return;
  const newText = $("editor").value;
  const op = diffToOp(lastText, newText);
  lastText = newText;
  if (!op) return;
  if (Array.isArray(op)) op.forEach(sendOp);
  else sendOp(op);
}

function setEditorText(text) {
  const ta = $("editor");
  const caret = ta.selectionStart;
  applyingRemote = true;
  ta.value = text;
  lastText = text;
  ta.selectionStart = ta.selectionEnd = Math.min(caret, text.length);
  applyingRemote = false;
}

async function resyncRest() {
  const r = await fetch(`${REST}/docs/${encodeURIComponent(docId)}`);
  const doc = await r.json();
  serverSeq = doc.seq;
  setEditorText(doc.text);
  $("seq").textContent = `seq ${serverSeq}`;
}

function onMessage(raw) {
  const msg = JSON.parse(raw);
  switch (msg.type) {
    case "resync":
      serverSeq = msg.seq;
      setEditorText(msg.text);
      $("seq").textContent = `seq ${serverSeq}`;
      break;
    case "op.applied":
      if (msg.clientId === clientId && msg.seq === serverSeq + 1) {
        serverSeq = msg.seq;                 
      } else if (msg.seq === serverSeq + 1) {
        setEditorText(applyOp($("editor").value, msg.transformedOp));
        serverSeq = msg.seq;
      } else if (msg.seq > serverSeq + 1) {
        resyncRest();                        
      }
      $("seq").textContent = `seq ${serverSeq}`;
      break;
    case "annotation":
      renderAnnotation(msg);
      break;
    case "presence":
      renderPresence(msg);
      break;
    case "error":
      console.warn("erro do servidor:", msg.error);
      break;
  }
}

function renderAnnotation(msg) {
  if (msg.issues !== undefined) {
    $("spellcheck").innerHTML = msg.issues.map((i) =>
      `<li><span class="word">${i.word}</span> @${i.range[0]}–${i.range[1]}
       ${i.suggest && i.suggest.length ? `→ <span class="suggest">${i.suggest.join(", ")}</span>` : ""}</li>`
    ).join("") || "<li>nenhum problema</li>";
  } else if (msg.suggestions !== undefined) {
    $("format").innerHTML = msg.suggestions.map((s) =>
      `<li><span class="kind">${s.kind}</span> @${s.range[0]} — ${s.message}</li>`
    ).join("") || "<li>sem sugestões</li>";
  }
}

function renderPresence(msg) {
  if (msg.event === "left") presence.delete(msg.clientId);
  else presence.set(msg.clientId, msg.event);
  $("presence").innerHTML = [...presence.keys()].map((c) => `<li>${c}</li>`).join("");
}

function connect() {
  docId = $("docId").value.trim();
  clientId = $("clientId").value.trim();
  ws = new WebSocket(WS_URL);
  ws.onopen = () => {
    setStatus(true);
    ws.send(JSON.stringify({ type: "join", docId, clientId }));
    $("editor").disabled = false;
  };
  ws.onclose = () => setStatus(false);
  ws.onerror = () => setStatus(false);
  ws.onmessage = (e) => onMessage(e.data);
}

$("connectBtn").addEventListener("click", connect);
$("snapshotBtn").addEventListener("click", async () => {
  if (!docId) return;
  await fetch(`${REST}/docs/${encodeURIComponent(docId)}/save`, { method: "POST" });
});
$("editor").addEventListener("input", onEditorInput);

const params = new URLSearchParams(location.search);
if (params.get("doc")) $("docId").value = params.get("doc");
if (params.get("client")) $("clientId").value = params.get("client");
if (params.get("doc")) connect();
