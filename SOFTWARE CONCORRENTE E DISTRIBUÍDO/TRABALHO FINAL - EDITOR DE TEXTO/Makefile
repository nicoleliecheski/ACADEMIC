COMPOSE ?= docker compose
PYTHON  ?= python3

.PHONY: up down build logs ps demo test clean cluster shardmap

up: 
	$(COMPOSE) up -d --build
	bash scripts/wait_for_health.sh

build:
	$(COMPOSE) build

down: 
	$(COMPOSE) down -v

logs: 
	$(COMPOSE) logs -f

ps: 
	$(COMPOSE) ps

cluster: 
	curl -fsS http://localhost:8080/cluster | $(PYTHON) -m json.tool

shardmap:
	curl -fsS http://localhost:8080/shardmap | $(PYTHON) -m json.tool

demo: 
	bash scripts/demo.sh

test:
	@cd doc_service && ($(PYTHON) -m pytest tests -q 2>/dev/null || $(PYTHON) tests/run_ops.py)

clean: down 
