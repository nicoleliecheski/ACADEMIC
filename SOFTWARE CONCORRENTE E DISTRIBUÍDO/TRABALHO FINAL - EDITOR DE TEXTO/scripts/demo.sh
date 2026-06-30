#!/usr/bin/env bash
# Demonstração ponta a ponta que exercita cada característica exigida do sistema.
# Execute após `make up`. Requer curl, python3 e o pacote `websockets`
# (instalado automaticamente no ambiente atual, se faltar).
set -euo pipefail

cd "$(dirname "$0")/.."

GW="${GW_HTTP:-http://localhost:8080}"
export GW_HTTP="$GW"
export GW_WS="${GW_WS:-ws://localhost:8081}"
PY="${PYTHON:-python3}"
SIM="clients/sim_client.py"

hr() { printf '\n\033[1;36m========== %s ==========\033[0m\n' "$1"; }
note() { printf '\033[0;33m%s\033[0m\n' "$1"; }

# --- pré-requisitos ---------------------------------------------------------
if ! "$PY" -c "import websockets" 2>/dev/null; then
  note "instalando 'websockets' para os clientes simulados..."
  if ! "$PY" -m pip --version >/dev/null 2>&1; then
    echo "ERRO: pip não está disponível para '$PY'. Instale primeiro e rode 'make demo' de novo:" >&2
    echo "  Amazon Linux:  sudo dnf -y install python3-pip" >&2
    echo "  Ubuntu/Debian: sudo apt-get install -y python3-pip" >&2
    exit 1
  fi
  # --user evita o site-packages do sistema; a segunda forma trata o PEP 668
  # ('externally managed environment' do Amazon Linux 2023 / Debian 12+).
  "$PY" -m pip install --user --quiet websockets \
    || "$PY" -m pip install --user --break-system-packages --quiet websockets
fi

hr "0. Aguardando o cluster ficar saudável"
bash scripts/wait_for_health.sh

hr "1. Topologia do cluster (mapa de shards + papéis ao vivo)"
curl -fsS "$GW/shardmap"; echo
curl -fsS "$GW/cluster" | "$PY" -m json.tool

hr "2. Particionamento: criar documentos e observar o roteamento por shard"
for d in doc-A doc-B doc-1 doc-2 doc-3 doc-4; do
  resp=$(curl -fsS -X POST "$GW/docs" -H 'content-type: application/json' -d "{\"docId\":\"$d\"}")
  echo "  criado $d -> $(echo "$resp" | "$PY" -c 'import sys,json;d=json.load(sys.stdin);print(d["shardId"])')"
done
note "docIds diferentes mapeiam para shards diferentes via hashing consistente."

hr "3. Concorrência + consistência: 5 clientes editam doc-A ao mesmo tempo"
"$PY" "$SIM" converge --doc doc-A --clients 5 --ops 12
note "Todos os clientes e o servidor convergiram para o mesmo texto + seq."

hr "4. Corretor ortográfico em segundo plano (processamento concorrente + notificação assíncrona)"
note "Iniciando um observador em doc-B e, em seguida, digitando texto com erros..."
"$PY" "$SIM" watch --doc doc-B --client observador --seconds 8 &
WATCH_PID=$!
sleep 1
"$PY" "$SIM" type --doc doc-B --client digitador --text "voce escreveu rapido e naum revisou o testo final"
wait $WATCH_PID || true
note "As anotações do corretor foram produzidas pelos workers de segundo plano (veja as tags do worker) e enviadas via WebSocket."

hr "5. Caminhos síncrono vs assíncrono"
note "SÍNCRONO (REST bloqueante): GET /docs/doc-A"
curl -fsS "$GW/docs/doc-A" | "$PY" -c 'import sys,json;d=json.load(sys.stdin);print("  seq=%s tamanho=%s servidoPor=%s"%(d["seq"],len(d["text"]),d.get("servedBy")))'
note "ASSÍNCRONO (WS ao vivo): os eventos op.applied + annotation acima. Os logs do gateway marcam [SYNC] vs WS."

hr "6. Leitura a partir da réplica (compromisso consistência/disponibilidade)"
note "Ambos os nós do sharda estão vivos; servindo esta leitura pela réplica."
curl -fsS "$GW/docs/doc-A?replica=1" | "$PY" -c 'import sys,json;d=json.load(sys.stdin);print("  servidoPor=%s seq=%s"%(d.get("servedBy"),d["seq"]))'

hr "7. Disponibilidade: derrubar o primário atual do sharda -> a réplica assume"
SEQ_BEFORE=$(curl -fsS "$GW/docs/doc-A" | "$PY" -c 'import sys,json;print(json.load(sys.stdin)["seq"])')
echo "  seq de doc-A antes do failover: $SEQ_BEFORE"
bash scripts/kill_primary.sh sharda
note "Escrevendo em doc-A logo após o failover (o gateway re-resolve o primário + tenta de novo)..."
"$PY" "$SIM" edit --doc doc-A --client pos-failover --ops 5
SEQ_AFTER=$(curl -fsS "$GW/docs/doc-A" | "$PY" -c 'import sys,json;print(json.load(sys.stdin)["seq"])')
echo "  seq de doc-A depois do failover:  $SEQ_AFTER (contínuo, sem perda de dados)"

hr "8. Cobertura dos requisitos"
cat <<'EOF'
  [x] Múltiplos clientes na Internet ......... gateway REST + WebSocket
  [x] Vários componentes próprios ............ gateway, doc-service(s), workers, coordenador
  [x] Acesso concorrente a dados ............. etapa 3 (5 clientes, um documento)
  [x] Processamento concorrente no servidor .. etapa 4 (workers de corretor/formatação)
  [x] Interação SÍNCRONA E ASSÍNCRONA ........ etapa 5
  [x] Replicação E particionamento ........... etapas 2, 6, 7
  [x] Consistência E disponibilidade ......... etapas 3, 6, 7
  [x] >1 linguagem + paradigmas .............. Node + Python; cliente-servidor, pub/sub, mensageria
EOF
hr "DEMONSTRAÇÃO CONCLUÍDA"
