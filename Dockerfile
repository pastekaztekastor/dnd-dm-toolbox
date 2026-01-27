# Multi-stage build for DnD DM Toolbox

# Stage 1: Build environment
FROM ubuntu:22.04 AS builder

# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libglfw3-dev \
    libgl1-mesa-dev \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Download dependencies
RUN chmod +x setup_dependencies.sh && ./setup_dependencies.sh

# Build the project
RUN mkdir -p build && cd build && \
    cmake .. && \
    make -j$(nproc)

# Stage 2: Runtime environment with X11 support
FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libglfw3 \
    libgl1-mesa-glx \
    libx11-6 \
    libxrandr2 \
    libxinerama1 \
    libxcursor1 \
    libxi6 \
    x11-apps \
    && rm -rf /var/lib/apt/lists/*

# Create app user
RUN useradd -m -s /bin/bash dmuser

WORKDIR /app

# Copy built executable and resources from builder
COPY --from=builder /app/build/DnD_DM_Toolbox /app/
COPY --from=builder /app/build/resources /app/resources
COPY --from=builder /app/build/data /app/data

# Change ownership
RUN chown -R dmuser:dmuser /app

USER dmuser

# Set display for X11
ENV DISPLAY=:0

CMD ["./DnD_DM_Toolbox"]
