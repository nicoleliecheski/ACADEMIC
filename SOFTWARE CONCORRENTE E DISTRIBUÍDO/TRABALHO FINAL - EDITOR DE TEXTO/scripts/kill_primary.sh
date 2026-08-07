set -euo pipefail

SHARD="${1:-sharda}"
COMPOSE="docker compose"

lease=$($COMPOSE exec -T redis redis-cli get "lease:shard:${SHARD}" | tr -d '\r')
if [ -z "$lease" ]; then
  echo "no current primary for $SHARD (lease empty)"; exit 1
fi
container="${lease%%:*}"
echo "current primary of $SHARD is '$container' (lease=$lease) -- killing it"
$COMPOSE kill "$container"

echo "waiting for a replica to take over the lease ..."
for i in $(seq 1 20); do
  new=$($COMPOSE exec -T redis redis-cli get "lease:shard:${SHARD}" | tr -d '\r')
  if [ -n "$new" ] && [ "$new" != "$lease" ]; then
    echo "NEW primary of $SHARD is '$new' ✅"
    exit 0
  fi
  sleep 1
done
echo "no takeover observed within timeout" >&2
exit 1
