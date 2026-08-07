FROM python:3.11-slim

WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

CMD ["sh", "-c", "if [ \"$WORKER\" = format ]; then python formatter_worker.py; else python spellcheck_worker.py; fi"]
