FROM node:20-slim

WORKDIR /app
COPY package.json .
RUN npm install --omit=dev

COPY src ./src

# REST/UI on 8080, WebSocket on 8081 (overridable via env).
EXPOSE 8080 8081
CMD ["node", "src/server.js"]
