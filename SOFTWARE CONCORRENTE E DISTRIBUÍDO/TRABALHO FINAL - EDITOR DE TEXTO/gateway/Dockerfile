FROM node:20-slim

WORKDIR /app
COPY package.json .
RUN npm install --omit=dev

COPY src ./src

# REST/UI na 8080, WebSocket na 8081 (sobrescrevíveis via env).
EXPOSE 8080 8081
CMD ["node", "src/server.js"]
