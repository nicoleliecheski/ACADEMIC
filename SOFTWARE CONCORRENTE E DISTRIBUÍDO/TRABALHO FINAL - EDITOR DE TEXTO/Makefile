# Alvos de conveniência para o editor de documentos compartilhado.
# Usa Docker Compose v2 ("docker compose"). Sobrescreva com COMPOSE=...

COMPOSE ?= docker compose
PYTHON  ?= python3

.PHONY: up down build logs ps demo test clean cluster shardmap

up: ## Builda as imagens e sobe todo o cluster
	$(COMPOSE) up -d --build
	bash scripts/wait_for_health.sh

build: ## Apenas builda as imagens
	$(COMPOSE) build

down: ## Para e remove containers + volumes
	$(COMPOSE) down -v

logs: ## Acompanha os logs de todos os serviços
	$(COMPOSE) logs -f

ps: ## Mostra o status dos containers
	$(COMPOSE) ps

cluster: ## Mostra os papéis primário/réplica por shard, ao vivo
	curl -fsS http://localhost:8080/cluster | $(PYTHON) -m json.tool

shardmap: ## Mostra o mapa de shards
	curl -fsS http://localhost:8080/shardmap | $(PYTHON) -m json.tool

demo: ## Roda o cenário de demonstração completo e narrado
	bash scripts/demo.sh

test: ## Roda os testes de consistência/rebase (pytest se houver, senão fallback)
	@cd doc_service && ($(PYTHON) -m pytest tests -q 2>/dev/null || $(PYTHON) tests/run_ops.py)

clean: down ## Atalho para down
