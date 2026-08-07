set -euo pipefail

BUILDX_VERSION="v0.17.1"

if command -v dnf >/dev/null 2>&1; then        
  sudo dnf -y update
  sudo dnf -y install docker git make python3-pip
  sudo systemctl enable --now docker
  sudo usermod -aG docker "$USER"
elif command -v apt-get >/dev/null 2>&1; then 
  sudo apt-get update
  sudo apt-get install -y docker.io git make python3-pip
  sudo systemctl enable --now docker
  sudo usermod -aG docker "$USER"
else
  echo "unsupported package manager; install Docker + Compose manually" >&2
  exit 1
fi

python3 -m pip install --user websockets \
  || python3 -m pip install --user --break-system-packages websockets || true

case "$(uname -m)" in
  x86_64)  ARCH=amd64 ;;
  aarch64) ARCH=arm64 ;;
  *) echo "unknown arch $(uname -m)" >&2; exit 1 ;;
esac

PLUGINS=/usr/local/lib/docker/cli-plugins
sudo mkdir -p "$PLUGINS"

sudo curl -fsSL "https://github.com/docker/compose/releases/latest/download/docker-compose-$(uname -s)-$(uname -m)" \
  -o "$PLUGINS/docker-compose"
sudo chmod +x "$PLUGINS/docker-compose"

sudo curl -fsSL "https://github.com/docker/buildx/releases/download/${BUILDX_VERSION}/buildx-${BUILDX_VERSION}.linux-${ARCH}" \
  -o "$PLUGINS/docker-buildx"
sudo chmod +x "$PLUGINS/docker-buildx"

echo "Installed:"
docker --version || true
docker compose version || true
docker buildx version || true
echo
echo "Docker ready. Log out/in (or run 'newgrp docker'), then: make up"
