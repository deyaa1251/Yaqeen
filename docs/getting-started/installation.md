# Installation

Yaqeen can be installed in several ways depending on your platform and preferences.

## Pre-built Binaries

### Linux

**Ubuntu/Debian:**
```bash
# Coming soon
# sudo apt install yaqeen
```

**Arch Linux:**
```bash
# Coming soon
# yay -S yaqeen
```

**Generic Linux:**
Download the latest binary from [releases](https://github.com/yourusername/yaqeen/releases):
```bash
wget https://github.com/yourusername/yaqeen/releases/latest/download/yaqeen-linux-x64
chmod +x yaqeen-linux-x64
sudo mv yaqeen-linux-x64 /usr/local/bin/yaqeen
```

### macOS

**Using Homebrew:**
```bash
# Coming soon
# brew install yaqeen
```

**Manual installation:**
```bash
curl -L https://github.com/yourusername/yaqeen/releases/latest/download/yaqeen-macos -o yaqeen
chmod +x yaqeen
sudo mv yaqeen /usr/local/bin/
```

### Windows

**Using Scoop:**
```powershell
# Coming soon
# scoop install yaqeen
```

**Using Chocolatey:**
```powershell
# Coming soon
# choco install yaqeen
```

**Manual installation:**
1. Download `yaqeen-windows.exe` from [releases](https://github.com/yourusername/yaqeen/releases)
2. Add to your PATH or move to a directory in PATH

## Building from Source

### Requirements

- C++17 compatible compiler:
  - GCC 7 or higher
  - Clang 5 or higher
  - MSVC 2017 or higher
- CMake 3.14 or higher
- Git

### Linux/macOS

```bash
# Clone the repository
git clone https://github.com/yourusername/yaqeen.git
cd yaqeen

# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build (use -j for parallel compilation)
make -j$(nproc)

# Install
sudo make install
```

### Windows

Using Visual Studio:

```powershell
# Clone the repository
git clone https://github.com/yourusername/yaqeen.git
cd yaqeen

# Create build directory
mkdir build
cd build

# Configure (adjust generator as needed)
cmake .. -G "Visual Studio 16 2019"

# Build
cmake --build . --config Release

# Install
cmake --install . --config Release
```

## Docker

Run Yaqeen in a container:

```bash
docker pull yaqeen/yaqeen:latest

# Use it
docker run --rm -v $(pwd):/workspace yaqeen/yaqeen create --template react-typescript --name my-app
```

## Verifying Installation

Check that Yaqeen is installed correctly:

```bash
yaqeen --version
```

You should see output like:
```
Yaqeen version 1.0.0
```

## Next Steps

Now that Yaqeen is installed, check out the [Quick Start Guide](quick-start.md) to create your first project!
