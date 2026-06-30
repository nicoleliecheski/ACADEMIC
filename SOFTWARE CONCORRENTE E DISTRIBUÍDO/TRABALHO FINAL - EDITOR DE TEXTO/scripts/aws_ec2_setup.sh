#!/usr/bin/env bash
# One-time provisioning for a fresh EC2 instance (Amazon Linux 2023 or Ubuntu).
# Installs Docker, git, make, and CURRENT Compose + buildx plugins, then enables
# the service. Re-login (or `newgrp docker`) afterwards so your user can run
# docker without sudo.
#
# Why install buildx/compose by hand: the distro-packaged Docker on Amazon Linux
# 2023 ships a buildx older than 0.17.0, and Compose v2 `--build` then fails with
# "compose build requires buildx 0.17.0 or later".
set -euo pipefail

# Pin a buildx version that satisfies Compose's >= 0.17.0 requirement.
BUILDX_VERSION="v0.17.1"

if command -v dnf >/dev/null 2>&1; then        # Amazon Linux 2023 / Fedora
  sudo dnf -y update
  sudo dnf -y install docker git make python3-pip
  sudo systemctl enable --now docker
  sudo usermod -aG docker "$USER"
elif command -v apt-get >/dev/null 2>&1; then  # Ubuntu / Debian
  sudo apt-get update
  sudo apt-get install -y docker.io git make python3-pip
  sudo systemctl enable --now docker
  sudo usermod -aG docker "$USER"
else
  echo "unsupported package manager; install Docker + Compose manually" >&2
  exit 1
fi

# Simulated demo clients (run on the host) need the 'websockets' package.
python3 -m pip install --user websockets \
  || python3 -m pip install --user --break-system-packages websockets || true

# --- Install current Compose + buildx CLI plugins (system-wide) --------------
case "$(uname -m)" in
  x86_64)  ARCH=amd64 ;;
  aarch64) ARCH=arm64 ;;
  *) echo "unknown arch $(uname -m)" >&2; exit 1 ;;
esac

PLUGINS=/usr/local/lib/docker/cli-plugins
sudo mkdir -p "$PLUGINS"

# Compose (asset name has no version, so latest/download works).
sudo curl -fsSL "https://github.com/docker/compose/releases/latest/download/docker-compose-$(uname -s)-$(uname -m)" \
  -o "$PLUGINS/docker-compose"
sudo chmod +x "$PLUGINS/docker-compose"

# buildx (asset name is versioned).
sudo curl -fsSL "https://github.com/docker/buildx/releases/download/${BUILDX_VERSION}/buildx-${BUILDX_VERSION}.linux-${ARCH}" \
  -o "$PLUGINS/docker-buildx"
sudo chmod +x "$PLUGINS/docker-buildx"

echo "Installed:"
docker --version || true
docker compose version || true
docker buildx version || true
echo
echo "Docker ready. Log out/in (or run 'newgrp docker'), then: make up"
