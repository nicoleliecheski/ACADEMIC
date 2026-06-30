# Roteiro do Vídeo de Apresentação — UnifiDocs

Vídeo de até **8 minutos**, apresentado por **4 pessoas com partes iguais
(~2 min cada)**. O roteiro garante que **todas as 7 características exigidas** do
trabalho sejam demonstradas e destacadas.

> **Mapa das características → apresentador**
> - **P1:** (1) múltiplos clientes na Internet · (2) vários componentes distribuídos próprios
> - **P2:** (3) acessos concorrentes · (7a) consistência
> - **P3:** (4) processamento no servidor concorrente · (5) síncrono **e** assíncrono
> - **P4:** (6) replicação **e** particionamento · (7b) disponibilidade · fechamento

---

## Preparação (antes de gravar)

- Na EC2: `make up` e aguardar ficar saudável. Ter **3 terminais** abertos:
  - **T1** – comandos de cliente (`curl`, `sim_client.py`)
  - **T2** – logs ao vivo: `docker compose logs -f gateway sharda-primary worker-spell-1`
  - **T3** – failover (`scripts/kill_primary.sh`)
- Navegador aberto em `http://<IP_PUBLICO_EC2>:8080` (e uma 2ª aba pronta).
- Deixe `make cluster` e `make shardmap` à mão.
- Pré-aqueça os comandos uma vez antes de gravar (a 1ª execução do `converge`/build é mais lenta).

---

## P1 — Abertura, acesso pela Internet e componentes distribuídos (0:00–2:00)

**Destaca: (1) múltiplos clientes na Internet · (2) componentes distribuídos próprios**

**Fala:**
> "Olá! Somos o grupo do **UnifiDocs**, um **editor de documentos colaborativo**
> concorrente e distribuído. Vários usuários editam o mesmo documento ao mesmo
> tempo, veem as edições ao vivo e contam com um corretor ortográfico em
> português rodando no servidor.
> Primeiro, a característica de **serviço acessível a múltiplos clientes na
> Internet**: tudo está rodando numa instância **AWS EC2**, e este navegador no
> meu computador acessa o serviço pelo **IP público** — REST na porta 8080 e
> WebSocket na 8081.
> Segundo, o sistema é **composto por vários componentes distribuídos que nós
> implementamos**. Olhem o cluster: temos um **gateway em Node.js** (borda REST +
> WebSocket), o **serviço de documentos em Python** replicado em dois shards,
> **workers em Python** para corretor e formatação, e o **Redis** coordenando
> tudo. São duas linguagens e três paradigmas: cliente-servidor,
> publish-subscribe e mensageria."

**Na tela:**
1. Abrir a UI pelo IP público (mostra que é remoto).
2. `make cluster` → mostra sharda/shardb com primário e réplica.
3. `docker compose ps` → lista gateway, 2×(primário+réplica), workers, redis.

---

## P2 — Acessos concorrentes e consistência (2:00–4:00)

**Destaca: (3) acessos concorrentes a dados compartilhados · (7a) consistência**

**Fala:**
> "Agora os **acessos concorrentes a dados compartilhados**. Vou abrir **duas
> abas** no mesmo documento `doc-A` e digitar nas duas ao mesmo tempo — reparem
> que as edições aparecem instantaneamente de um lado para o outro.
> Para estressar de verdade, rodo um teste com **5 clientes simulados** editando
> o mesmo documento simultaneamente. Cada cliente envia operações concorrentes.
> Como o sistema usa um **sequenciador central com log de operações** — o
> primário do shard atribui uma ordem total e faz o *rebase* das edições
> concorrentes — todos convergem para **exatamente o mesmo texto e o mesmo número
> de sequência**. Esse é o nosso **tratamento de consistência**: o resultado
> `CONVERGÊNCIA: PASSOU` confirma que os 5 clientes e o servidor ficaram
> idênticos. Nada de edição perdida ou divergente."

**Na tela:**
1. Duas abas em `doc-A`, digitando nas duas (colaboração ao vivo).
2. `python3 clients/sim_client.py converge --doc doc-A --clients 5 --ops 12`
   → mostra `CONVERGÊNCIA: PASSOU ✅` e o mesmo `seq` para todos.

---

## P3 — Processamento no servidor + síncrono/assíncrono (4:00–6:00)

**Destaca: (4) processamento no servidor concorrente com os acessos · (5) interação síncrona e assíncrona**

**Fala:**
> "Enquanto os clientes editam, o servidor faz **processamento em segundo plano,
> concorrentemente**. Temos workers em Python que recebem o texto por uma fila no
> Redis e rodam um **corretor ortográfico em português**. Vou digitar com erros
> propositais — `voce escreveu rapido` — e em segundos as **anotações chegam ao
> vivo** no painel, marcando `voce` e `rapido` com as sugestões `você` e
> `rápido`. Isso acontece em **workers separados**, sem travar a edição.
> Aqui também aparecem os **dois mecanismos de interação**: o **síncrono e
> bloqueante** — abrir e salvar o documento via **REST** — e o **assíncrono** —
> as edições e as anotações que chegam via **WebSocket e Redis pub/sub**, sem o
> cliente ficar esperando. Nos logs dá pra ver: as chamadas marcadas como
> `[SYNC]` são REST; o fluxo de `op.applied` e `annotation` é todo assíncrono."

**Na tela:**
1. T2 (logs) visível. Digitar `voce escreveu rapido` na UI → anotações aparecem no painel **Ortografia**.
2. Clicar **Salvar** (REST síncrono) → mostrar linha `[SYNC] POST .../save` no log.
3. Apontar no log o fluxo assíncrono `op.applied` / `annotation`.

---

## P4 — Replicação, particionamento, disponibilidade e fechamento (6:00–8:00)

**Destaca: (6) replicação e particionamento · (7b) disponibilidade**

**Fala:**
> "Por fim, **replicação e particionamento**. Os documentos são **particionados**
> por hashing consistente entre dois shards, e cada shard é **replicado** — tem
> um primário e uma réplica. Dá até pra ler direto da réplica com `?replica=1`.
> Agora o **tratamento de disponibilidade**: vou **derrubar o primário** do
> sharda no meio da operação. O lease no Redis expira, a **réplica é promovida
> automaticamente**, o gateway redireciona e **continua aceitando escritas** —
> reparem que o número de sequência segue **contínuo, sem perda de dados**. O
> serviço se manteve disponível mesmo com a queda de um nó.
> Recapitulando: o **UnifiDocs** atende às sete características — múltiplos
> clientes na Internet, vários componentes distribuídos próprios, acesso
> concorrente, processamento no servidor concorrente, interação síncrona e
> assíncrona, replicação e particionamento, e consistência com disponibilidade —
> em Node.js, Python e Redis, sobre AWS EC2. Obrigado!"

**Na tela:**
1. `make shardmap` + criar `doc-A`/`doc-B` mostrando shards diferentes.
2. `curl 'localhost:8080/docs/doc-A?replica=1'` → `servedBy=replica`.
3. T3: `scripts/kill_primary.sh sharda` → "NEW primary ... ✅".
4. Escrever em `doc-A` após a queda e mostrar `seq` contínuo (`make cluster` confirmando o novo primário).

---

## Dicas de tempo

- Cada bloco tem ~2 min: **fale enquanto a demo roda** (não espere comandos terminarem em silêncio).
- Se algo falhar ao vivo, o `make demo` executa **todo** esse roteiro narrado
  automaticamente — bom plano B para gravar a tela.

## Comandos usados (referência rápida)

```bash
make up                                   # subir o cluster
make cluster                              # papéis primário/réplica por shard
make shardmap                             # mapa de particionamento
docker compose ps                         # listar componentes
docker compose logs -f gateway sharda-primary worker-spell-1

# clientes
python3 clients/sim_client.py converge --doc doc-A --clients 5 --ops 12
python3 clients/sim_client.py type --doc doc-A --client t1 --text "voce escreveu rapido"
curl 'http://localhost:8080/docs/doc-A?replica=1'

# disponibilidade
scripts/kill_primary.sh sharda

# plano B: demonstração completa automática
make demo
```
