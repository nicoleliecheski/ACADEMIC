FROM python:3.11-slim

WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

# PORT vem por env (padrão 9000). Um único worker: o nó mantém estado
# autoritativo em memória, então não pode ser bifurcado horizontalmente.
CMD ["sh", "-c", "uvicorn app:app --host 0.0.0.0 --port ${PORT:-9000} --log-level warning"]
