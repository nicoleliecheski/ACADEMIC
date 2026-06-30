FROM python:3.11-slim

WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY . .
# The dictionary file is mounted/copied to /app/words.txt via compose.

# WORKER selects which worker to run: "spell" or "format".
CMD ["sh", "-c", "if [ \"$WORKER\" = format ]; then python formatter_worker.py; else python spellcheck_worker.py; fi"]
