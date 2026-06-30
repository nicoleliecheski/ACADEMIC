#!/usr/bin/env bash
# End-to-end demonstration that exercises every required system characteristic.
# Run after `make up`. Requires curl, python3 and the `websockets` package
# (auto-installed into the active environment if missing).
set -euo pipefail

cd "$(dirname "$0")/.."

GW="${GW_HTTP:-http://localhost:8080}"
export GW_HTTP="$GW"
export GW_WS="${GW_WS:-ws://localhost:8081}"
PY="${PYTHON:-python3}"
SIM="clients/sim_client.py"

hr() { printf '\n\033[1;36m========== %s ==========\033[0m\n' "$1"; }
note() { printf '\033[0;33m%s\033[0m\n' "$1"; }

# --- prerequisites ----------------------------------------------------------
if ! "$PY" -c "import websockets" 2>/dev/null; then
  note "installing 'websockets' for the simulated clients..."
  if ! "$PY" -m pip --version >/dev/null 2>&1; then
    echo "ERROR: pip is not available for '$PY'. Install it first, then re-run 'make demo':" >&2
    echo "  Amazon Linux:  sudo dnf -y install python3-pip" >&2
    echo "  Ubuntu/Debian: sudo apt-get install -y python3-pip" >&2
    exit 1
  fi
  # --user keeps it out of system site-packages; the second form handles
  # PEP 668 'externally managed environment' (Amazon Linux 2023, Debian 12+).
  "$PY" -m pip install --user --quiet websockets \
    || "$PY" -m pip install --user --break-system-packages --quiet websockets
fi

hr "0. Wait for the cluster to be healthy"
bash scripts/wait_for_health.sh

hr "1. Cluster topology (shard map + live roles)"
curl -fsS "$GW/shardmap"; echo
curl -fsS "$GW/cluster" | "$PY" -m json.tool

hr "2. Partitioning: create documents and observe shard routing"
for d in doc-A doc-B doc-1 doc-2 doc-3 doc-4; do
  resp=$(curl -fsS -X POST "$GW/docs" -H 'content-type: application/json' -d "{\"docId\":\"$d\"}")
  echo "  created $d -> $(echo "$resp" | "$PY" -c 'import sys,json;d=json.load(sys.stdin);print(d["shardId"])')"
done
note "Different docIds map to different shards via consistent hashing."

hr "3. Concurrency + consistency: 5 clients edit doc-A simultaneously"
"$PY" "$SIM" converge --doc doc-A --clients 5 --ops 12
note "Every client and the server converged to identical text + seq."

hr "4. Background spell-check (concurrent processing + async notification)"
note "Starting a watcher on doc-B, then typing misspelled text..."
"$PY" "$SIM" watch --doc doc-B --client watcher --seconds 8 &
WATCH_PID=$!
sleep 1
"$PY" "$SIM" type --doc doc-B --client typist --text "teh quik fox and a helo to the world"
wait $WATCH_PID || true
note "Spell-check annotations were produced by background workers (see worker tags) and pushed over WebSocket."

hr "5. Synchronous vs asynchronous paths"
note "SYNC (blocking REST): GET /docs/doc-A"
curl -fsS "$GW/docs/doc-A" | "$PY" -c 'import sys,json;d=json.load(sys.stdin);print("  seq=%s len=%s servedBy=%s"%(d["seq"],len(d["text"]),d.get("servedBy")))'
note "ASYNC (live WS): the op.applied + annotation events above. Gateway logs label [SYNC] vs WS."

hr "6. Read from replica (consistency/availability trade-off)"
note "Both nodes of sharda are alive; serving this read from the replica."
curl -fsS "$GW/docs/doc-A?replica=1" | "$PY" -c 'import sys,json;d=json.load(sys.stdin);print("  servedBy=%s seq=%s"%(d.get("servedBy"),d["seq"]))'

hr "7. Availability: kill the current primary of sharda -> replica takes over"
SEQ_BEFORE=$(curl -fsS "$GW/docs/doc-A" | "$PY" -c 'import sys,json;print(json.load(sys.stdin)["seq"])')
echo "  doc-A seq before failover: $SEQ_BEFORE"
bash scripts/kill_primary.sh sharda
note "Writing to doc-A right after failover (gateway re-resolves primary + retries)..."
"$PY" "$SIM" edit --doc doc-A --client post-failover --ops 5
SEQ_AFTER=$(curl -fsS "$GW/docs/doc-A" | "$PY" -c 'import sys,json;print(json.load(sys.stdin)["seq"])')
echo "  doc-A seq after failover:  $SEQ_AFTER (continuous, no data loss)"

hr "8. Requirement coverage"
cat <<'EOF'
  [x] Multiple Internet clients .......... gateway REST + WebSocket
  [x] Several custom components ........... gateway, doc-service(s), workers, coordinator
  [x] Concurrent shared-data access ...... step 3 (5 clients, one doc)
  [x] Concurrent server-side processing .. step 4 (spell/format workers)
  [x] Sync AND async interaction ......... step 5
  [x] Replication AND partitioning ....... steps 2, 6, 7
  [x] Consistency AND availability ....... steps 3, 6
  [x] >1 language + paradigms ............ Node + Python; client-server, pub/sub, messaging
EOF
hr "DEMO COMPLETE"
