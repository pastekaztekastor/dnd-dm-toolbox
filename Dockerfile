# Image de compilation - toutes les dépendances sont embarquées
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    pkg-config \
    libglfw3-dev \
    libgl1-mesa-dev \
    libsqlite3-dev \
    libgtk-3-dev \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libwayland-dev \
    wayland-protocols \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copier le projet (libs gitignorées seront téléchargées ci-dessous)
COPY . .

# Télécharger imgui (gitignorée)
RUN git clone --depth 1 https://github.com/ocornut/imgui.git libs/imgui

# Télécharger nlohmann/json (gitignorée)
RUN mkdir -p libs/json/include/nlohmann && \
    curl -sL https://github.com/nlohmann/json/releases/latest/download/json.hpp \
         -o libs/json/include/nlohmann/json.hpp

# Initialiser le submodule nfd
RUN git submodule update --init --recursive

# Compiler
RUN mkdir build && cd build && cmake .. && make -j$(nproc)

# ──────────────────────────────────────────────────────────────
# Image runtime légère (pour lancer l'app via X11)
FROM ubuntu:24.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    libglfw3 \
    libgl1-mesa-glx \
    libsqlite3-0 \
    libgtk-3-0 \
    libx11-6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/DnD_DM_Toolbox ./
COPY --from=builder /app/build/resources      ./resources
COPY --from=builder /app/build/plugins        ./plugins

ENV DISPLAY=:0

CMD ["./DnD_DM_Toolbox"]
