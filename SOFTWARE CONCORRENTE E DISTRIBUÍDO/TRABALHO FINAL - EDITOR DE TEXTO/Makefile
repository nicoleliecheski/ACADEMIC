# Convenience targets for the shared document editor.
# Uses Docker Compose v2 ("docker compose"). Override with COMPOSE=...

COMPOSE ?= docker compose
PYTHON  ?= python3

.PHONY: up down build logs ps demo test clean cluster shardmap

up: ## Build images and start the whole cluster
	$(COMPOSE) up -d --build
	bash scripts/wait_for_health.sh

build: ## Build images only
	$(COMPOSE) build

down: ## Stop and remove containers + volumes
	$(COMPOSE) down -v

logs: ## Follow logs from all services
	$(COMPOSE) logs -f

ps: ## Show container status
	$(COMPOSE) ps

cluster: ## Print live primary/replica roles per shard
	curl -fsS http://localhost:8080/cluster | $(PYTHON) -m json.tool

shardmap: ## Print the shard map
	curl -fsS http://localhost:8080/shardmap | $(PYTHON) -m json.tool

demo: ## Run the full narrated demonstration scenario
	bash scripts/demo.sh

test: ## Run the consistency/rebase unit tests (pytest if present, else fallback)
	@cd doc_service && ($(PYTHON) -m pytest tests -q 2>/dev/null || $(PYTHON) tests/run_ops.py)

clean: down ## Alias for down
