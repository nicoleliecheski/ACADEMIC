FROM python:3.11-slim

WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

# PORT is provided via env (defaults to 9000). Single worker: the node keeps
# authoritative in-memory state, so it must not be horizontally forked.
CMD ["sh", "-c", "uvicorn app:app --host 0.0.0.0 --port ${PORT:-9000} --log-level warning"]
