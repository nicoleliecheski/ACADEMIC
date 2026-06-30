#!/usr/bin/env bash
# One-time provisioning for a fresh EC2 instance (Amazon Linux 2023 or Ubuntu).
# Installs Docker + the Compose plugin and enables the service. Re-login (or
# `newgrp docker`) afterwards so your user can run docker without sudo.
set -euo pipefail

if command -v dnf >/dev/null 2>&1; then        # Amazon Linux 2023 / Fedora
  sudo dnf -y update
  sudo dnf -y install docker git make
  sudo systemctl enable --now docker
  sudo usermod -aG docker "$USER"
  # Compose plugin
  DOCKER_CONFIG=${DOCKER_CONFIG:-/usr/local/lib/docker}
  sudo mkdir -p "$DOCKER_CONFIG/cli-plugins"
  sudo curl -fsSL "https://github.com/docker/compose/releases/latest/download/docker-compose-$(uname -s)-$(uname -m)" \
    -o "$DOCKER_CONFIG/cli-plugins/docker-compose"
  sudo chmod +x "$DOCKER_CONFIG/cli-plugins/docker-compose"
elif command -v apt-get >/dev/null 2>&1; then  # Ubuntu / Debian
  sudo apt-get update
  sudo apt-get install -y docker.io docker-compose-v2 git make
  sudo systemctl enable --now docker
  sudo usermod -aG docker "$USER"
else
  echo "unsupported package manager; install Docker + Compose manually" >&2
  exit 1
fi

echo "Docker installed. Log out/in (or run 'newgrp docker'), then: make up"
