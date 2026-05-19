# D&D DM Toolbox

A comprehensive Dungeon Master toolbox for Dungeons & Dragons 5th Edition, built with C++ and Dear ImGui.

## Features

### Current Features
- Modern, dockable user interface powered by Dear ImGui
- Cross-platform support (Linux, Windows, macOS)
- Modular architecture for easy expansion

### Planned Features

#### Character Management
- Full D&D 5e character creation wizard
- Support for all official races and classes
- Character sheet viewer and editor
- Experience and level progression tracking
- Inventory and equipment management

#### Combat Tracker
- Initiative tracker with automatic sorting
- HP and temporary HP management
- Condition tracking (poisoned, paralyzed, etc.)
- Attack and damage roll calculators
- Turn-by-turn combat flow

#### Dungeon Generator
- Procedural dungeon generation
- Interactive map editor
- Room and corridor placement
- Random encounter tables
- Trap and treasure placement

#### Campaign Management
- Quest and scenario tracking
- NPC database
- Session notes and history
- World building tools

## Technology Stack

- **Language**: C++17
- **UI Framework**: Dear ImGui (docking branch)
- **Graphics**: OpenGL 3.3 + GLFW
- **Database**: PostgreSQL 15
- **Serialization**: nlohmann/json
- **Build System**: CMake
- **Containerization**: Docker & Docker Compose
- **CI/CD**: GitHub Actions

## Prerequisites

### Native Build
- CMake 3.15 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- GLFW3 development libraries
- OpenGL development libraries
- Git

#### Linux (Arch/Manjaro)
```bash
sudo pacman -S cmake gcc glfw-x11 mesa git
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get install cmake build-essential libglfw3-dev libgl1-mesa-dev libx11-dev git
```

### Docker Build
- Docker Engine 20.10+
- Docker Compose 2.0+

## Installation

### Option 1: Native Build

```bash
# Clone the repository
git clone https://github.com/yourusername/dnd-dm-toolbox.git
cd dnd-dm-toolbox

# Download dependencies
chmod +x setup_dependencies.sh
./setup_dependencies.sh

# Build the project
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run the application
./DnD_DM_Toolbox
```

### Option 2: Docker Development Environment

```bash
# Clone the repository
git clone https://github.com/yourusername/dnd-dm-toolbox.git
cd dnd-dm-toolbox

# Start the development environment
docker-compose up -d dev

# Enter the development container
docker-compose exec dev bash

# Inside the container
cd /workspace
./setup_dependencies.sh
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run with X11 forwarding
export DISPLAY=:0
./DnD_DM_Toolbox
```

### Option 3: Production Docker Build

```bash
# Build and run the production container
docker-compose --profile production up -d app

# Allow X11 connections (on host)
xhost +local:docker

# The application will start automatically
```

## Docker Services

The docker-compose setup includes:

- **postgres**: PostgreSQL database for campaign data storage
- **pgadmin**: Web-based database administration (http://localhost:5050)
- **dev**: Development environment with all build tools
- **app**: Production application container

### Database Access
- **Host**: localhost
- **Port**: 5432
- **Database**: dnd_toolbox
- **User**: dnduser
- **Password**: dndpass

### pgAdmin Access
- **URL**: http://localhost:5050
- **Email**: admin@dnd.local
- **Password**: admin

## Project Structure

```
```

## Development

### Building
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

### Running Tests
```bash
cd build
ctest --output-on-failure
```

### Code Style
We use clang-format for code formatting. Run before committing:
```bash
find src include -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Disclaimer

This is a fan-made tool for personal use. Dungeons & Dragons and all related content are property of Wizards of the Coast. This project is not affiliated with, endorsed, sponsored, or specifically approved by Wizards of the Coast LLC.

## Roadmap


## Support

For issues, questions, or suggestions, please open an issue on GitHub.

---

Made with ❤️ for Dungeon Masters everywhere
