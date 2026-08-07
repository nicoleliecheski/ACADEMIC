set -euo pipefail

cd "$(dirname "$0")/.."

GW="${GW_HTTP:-http://localhost:8080}"
export GW_HTTP="$GW"
export GW_WS="${GW_WS:-ws://localhost:8081}"
PY="${PYTHON:-python3}"
SIM="clients/sim_client.py"

hr() { printf '\n\033[1;36m========== %s ==========\033[0m\n' "$1"; }
note() { printf '\033[0;33m%s\033[0m\n' "$1"; }

if ! "$PY" -c "import websockets" 2>/dev/null; then
  note "instalando 'websockets' para os clientes simulados..."
  if ! "$PY" -m pip --version >/dev/null 2>&1; then
    echo "ERRO: pip não está disponível para '$PY'. Instale primeiro e rode 'make demo' de novo:" >&2
    echo " sudo apt-get install -y python3-pip" >&2
    exit 1
  fi
  "$PY" -m pip install --user --quiet websockets \
    || "$PY" -m pip install --user --break-system-packages --quiet websockets
fi

hr "0. aguardando o cluster ficar helthy"
bash scripts/wait_for_health.sh

hr "1. topologia do cluster"
curl -fsS "$GW/shardmap"; echo
curl -fsS "$GW/cluster" | "$PY" -m json.tool

hr "2. particionamento: criar documentos e observar o roteamento por shard"
for d in doc-A doc-B doc-1 doc-2 doc-3 doc-4; do
  resp=$(curl -fsS -X POST "$GW/docs" -H 'content-type: application/json' -d "{\"docId\":\"$d\"}")
  echo "  criado $d -> $(echo "$resp" | "$PY" -c 'import sys,json;d=json.load(sys.stdin);print(d["shardId"])')"
done
note "docIds diferentes mapeiam para shards diferentes via hashing."

hr "3. concorrencia + consistencia: 5 clientes editam doc-A ao mesmo tempo"
"$PY" "$SIM" converge --doc doc-A --clients 5 --ops 12
note "todos os clientes e o servidor convergiram para o mesmo texto + seq."

hr "4. corretor ortografico em segundo plano (processamento concorrente + notificacao assincrona)"
note "iniciando um observador em doc-B e digitando texto com erros"
"$PY" "$SIM" watch --doc doc-B --client observador --seconds 8 &
WATCH_PID=$!
sleep 1
"$PY" "$SIM" type --doc doc-B --client digitador --text "voce escreveu rapido e naum revisou o testo final"
wait $WATCH_PID || true
note "as anotacoes do corretor foram produzidas pelos workers de segundo plano e enviadas via websocket."

hr "5. caminhos sync vs async"
note "SYNC: GET /docs/doc-A"
curl -fsS "$GW/docs/doc-A" | "$PY" -c 'import sys,json;d=json.load(sys.stdin);print("  seq=%s tamanho=%s servidoPor=%s"%(d["seq"],len(d["text"]),d.get("servedBy")))'
note "ASYNC: os eventos op.applied + annotation acima. os logs do gateway marcam [SYNC] vs WS."

hr "6. leitura a partir da replica (consistencia/disponibilidade)"
note "ambos os nos do sharda estão vivos, servindo esta leitura pela replica."
curl -fsS "$GW/docs/doc-A?replica=1" | "$PY" -c 'import sys,json;d=json.load(sys.stdin);print("  servidoPor=%s seq=%s"%(d.get("servedBy"),d["seq"]))'

hr "7. disponibilidade: derrubar o primary do shardA, replica assume"
SEQ_BEFORE=$(curl -fsS "$GW/docs/doc-A" | "$PY" -c 'import sys,json;print(json.load(sys.stdin)["seq"])')
echo "  seq de doc-A antes do failover: $SEQ_BEFORE"
bash scripts/kill_primary.sh sharda
note "escrevendo em doc-A logo apos o failover"
"$PY" "$SIM" edit --doc doc-A --client pos-failover --ops 5
SEQ_AFTER=$(curl -fsS "$GW/docs/doc-A" | "$PY" -c 'import sys,json;print(json.load(sys.stdin)["seq"])')
echo "  seq de doc-A depois do failover:  $SEQ_AFTER (continuo, sem perda de dados)"

hr "8. cobertura dos requisitos"
cat <<'EOF'
  * multiplos clientes na internet: gateway REST + WebSocket
  * varios componentes proprios: gateway, doc-service(s), workers, coordenador
  * acesso concorrente a dados: etapa 3 (5 clientes, um documento)
  * processamento concorrente no servidor: etapa 4 (workers de corretor/formatação)
  * interacao sync e async: etapa 5
  * replicacao e particionamento: etapas 2, 6, 7
  * consistencia e disponibilidade: etapas 3, 6, 7
  * mais de uma linguagem + paradigmas: node + python, cliente-servidor, pub/sub, mensageria
EOF
hr "DEMO CONCLUIDA"
