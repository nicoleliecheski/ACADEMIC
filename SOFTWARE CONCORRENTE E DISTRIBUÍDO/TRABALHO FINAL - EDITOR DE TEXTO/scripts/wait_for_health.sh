set -euo pipefail

GW="${GW_HTTP:-http://localhost:8080}"
echo "waiting for gateway at $GW ..."
for i in $(seq 1 60); do
  if curl -fsS "$GW/health" >/dev/null 2>&1; then
    if curl -fsS "$GW/shardmap" | grep -q sharda; then
      echo "gateway healthy; shard map loaded."
      exit 0
    fi
  fi
  sleep 1
done
echo "gateway did not become healthy in time" >&2
exit 1
