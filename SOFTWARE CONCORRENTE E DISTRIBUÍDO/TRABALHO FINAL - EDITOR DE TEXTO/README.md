# UnifiDocs
Um editor de documentos em que vários clientes remotos visualizam e editam os mesmos documentos ao mesmo tempo. As edições são transmitidas ao vivo a cada participante via WebSocket, e workers de segundo plano em Python (corretor ortográfico, formatador) anotam o texto concorrentemente com a edição. O sistema é construído a partir de componentes distribuídos próprios em duas linguagens (Node.js + Python) coordenados através do Redis.

## Grupo Responsável
* Isabella Oliveira
* Nicole Liecheski
* Rafael Barbosa
* Pedro Lucas Barbosa

## Implantar na AWS EC2

1. Crie uma instância EC2 (Ubuntu, t3.small+).
2. TCP `8080` (REST + UI) e `8081` (WebSocket) a partir do seu IP (ou `0.0.0.0/0`).
3. Instale Docker + Compose.
4. Copie o projeto para a instância.
5. Execute:
   ```bash
   cd UnifiDocs
   make up
   ```
6. Acesse `http://<IP_EC2>:8080`. Para apontar os clientes simulados para a instância:
   ```bash
   GW_HTTP=http://<IP_EC2>:8080 GW_WS=ws://<IP_EC2>:8081 \
     python3 clients/sim_client.py converge --doc doc-A --clients 5 --ops 12
   ```
7. Rode `make demo` para demonstrar cada característica ponta a ponta.

## Pré-requisitos

* Docker + Docker Compose v2 (`docker compose`).
* Para os clientes simulados na EC2: Python 3.9 + `websockets` (`pip install -r clients/requirements.txt`). O script de demo instala automaticamente se faltar.

## Comandos
Todos tem que ser executados dentro da pasta do projeto.

```bash
make up        # builda as imagens e sobe o cluster 
make ps        # lista todos os containers
make cluster   # mostra os papeis por shard
make demo      # roda a demonstracao completa
make down      # para tudo
```

### Endpoints

```bash
curl localhost:8080/health
curl localhost:8080/shardmap
curl localhost:8080/cluster            
curl -X POST localhost:8080/docs -H 'content-type: application/json' -d '{"docId":"doc-A"}'
curl localhost:8080/docs/doc-A
curl 'localhost:8080/docs/doc-A?replica=1' 
```

### Clientes simulados

Clients python:

```bash
python3 -m pip install --user websockets   
python3 clients/sim_client.py create   --doc doc-A
python3 clients/sim_client.py converge --doc doc-A --clients 5 --ops 12   # prova de consistencia
python3 clients/sim_client.py type     --doc doc-A --client t1 --text "voce escreveu rapido"
python3 clients/sim_client.py watch    --doc doc-A --seconds 10           # anotacoes
```

Clients Node:

```bash
sudo dnf -y install nodejs 
( cd clients && npm install )     
node clients/sim_client.js edit --doc doc-A --client n1 --ops 15
```

### Failover

```bash
scripts/kill_primary.sh sharda   # derruba o primary e promove o clone
make cluster                     # pra ver o novo primary
```

## Testes

```bash
make test 
```

