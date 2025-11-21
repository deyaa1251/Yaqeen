# Building Yaqeen from Source

This guide provides detailed instructions for building Yaqeen on different platforms.

## Requirements

### All Platforms

- **C++17 compatible compiler**
  - GCC 7.0 or higher
  - Clang 5.0 or higher
  - MSVC 2017 or higher
  - AppleClang 10.0 or higher (macOS)

- **CMake** 3.14 or higher
- **Git** (for cloning and fetching dependencies)

### Optional

- **Ninja** - Faster build system (recommended)
- **ccache** - Compiler cache for faster rebuilds

## Linux

### Ubuntu/Debian

**Install dependencies:**
```bash
sudo apt update
sudo apt install -y build-essential cmake git ninja-build
```

**Build:**
```bash
# Clone repository
git clone https://github.com/yourusername/yaqeen.git
cd yaqeen

# Create build directory
mkdir build && cd build

# Configure with Ninja (faster)
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..

# Or with Make
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build
ninja  # or: make -j$(nproc)

# Run tests
ctest

# Install (optional)
sudo ninja install  # or: sudo make install
```

### Fedora/RHEL

**Install dependencies:**
```bash
sudo dnf install -y gcc-c++ cmake git ninja-build
```

Follow the same build steps as Ubuntu.

### Arch Linux

**Install dependencies:**
```bash
sudo pacman -S base-devel cmake git ninja
```

Follow the same build steps as Ubuntu.

## macOS

### Using Homebrew

**Install dependencies:**
```bash
brew install cmake ninja
```

**Build:**
```bash
# Clone repository
git clone https://github.com/yourusername/yaqeen.git
cd yaqeen

# Create build directory
mkdir build && cd build

# Configure
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..

# Build
ninja

# Run tests
ctest

# Install (optional)
sudo ninja install
```

### Using Xcode

**Install Xcode Command Line Tools:**
```bash
xcode-select --install
```

**Build:**
```bash
git clone https://github.com/yourusername/yaqeen.git
cd yaqeen
mkdir build && cd build

# Generate Xcode project
cmake -G Xcode ..

# Build
cmake --build . --config Release

# Or open in Xcode
open yaqeen.xcodeproj
```

## Windows

### Using Visual Studio

**Requirements:**
- Visual Studio 2017 or later with C++ development tools
- CMake (install via Visual Studio installer or standalone)
- Git for Windows

**Build with Command Line:**
```powershell
# Clone repository
git clone https://github.com/yourusername/yaqeen.git
cd yaqeen

# Create build directory
mkdir build
cd build

# Configure (adjust generator for your VS version)
cmake .. -G "Visual Studio 16 2019" -A x64

# Build
cmake --build . --config Release

# Run tests
ctest -C Release

# Install (run as Administrator)
cmake --install . --config Release
```

**Build with Visual Studio GUI:**
1. Open Visual Studio
2. File → Open → CMake
3. Select `yaqeen/CMakeLists.txt`
4. Build → Build All

### Using MSYS2/MinGW

**Install MSYS2** from https://www.msys2.org/

**In MSYS2 terminal:**
```bash
# Update packages
pacman -Syu

# Install build tools
pacman -S mingw-w64-x86_64-gcc \
          mingw-w64-x86_64-cmake \
          mingw-w64-x86_64-ninja \
          git

# Build
git clone https://github.com/yourusername/yaqeen.git
cd yaqeen
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
ctest
```

## Build Options

### CMake Options

```bash
# Build type
-DCMAKE_BUILD_TYPE=Debug|Release|RelWithDebInfo|MinSizeRel

# Disable tests
-DBUILD_TESTS=OFF

# Custom install prefix
-DCMAKE_INSTALL_PREFIX=/custom/path

# Use system libraries (if available)
-DUSE_SYSTEM_FTXUI=ON
-DUSE_SYSTEM_JSON=ON
```

### Build Configurations

**Debug build (for development):**
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

**Release build (optimized):**
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

**Release with debug info:**
```bash
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
make -j$(nproc)
```

**Minimal size build:**
```bash
cmake -DCMAKE_BUILD_TYPE=MinSizeRel ..
make -j$(nproc)
```

## Advanced Build Options

### Static Linking

Build with static libraries for portability:

```bash
cmake -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=OFF \
      ..
```

### Custom Compiler

```bash
# GCC
cmake -DCMAKE_CXX_COMPILER=g++-11 ..

# Clang
cmake -DCMAKE_CXX_COMPILER=clang++-14 ..
```

### Enable All Warnings

```bash
cmake -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic" ..
```

### Link Time Optimization (LTO)

```bash
cmake -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
      -DCMAKE_BUILD_TYPE=Release \
      ..
```

## Building with Docker

### Using Pre-built Image

```bash
docker pull yaqeen/yaqeen-builder:latest
docker run --rm -v $(pwd):/src yaqeen/yaqeen-builder
```

### Building Custom Image

**Dockerfile:**
```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git

WORKDIR /src
CMD ["bash", "-c", "mkdir -p build && cd build && cmake -G Ninja .. && ninja"]
```

**Build:**
```bash
docker build -t yaqeen-builder .
docker run --rm -v $(pwd):/src yaqeen-builder
```

## Troubleshooting

### Common Issues

**CMake version too old:**
```bash
# Install newer CMake
pip install --upgrade cmake
```

**Compiler not found:**
```bash
# Specify compiler explicitly
export CXX=/usr/bin/g++-10
cmake ..
```

**Missing dependencies:**
```bash
# Clean build directory and reconfigure
rm -rf build
mkdir build && cd build
cmake ..
```

**Link errors:**
```bash
# Try cleaning and rebuilding
make clean
make -j$(nproc)
```

### Build Performance

**Use Ninja (faster than Make):**
```bash
cmake -G Ninja ..
ninja
```

**Use ccache (caches compilation):**
```bash
# Install ccache
sudo apt install ccache  # Ubuntu
brew install ccache      # macOS

# Configure CMake to use ccache
cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache ..
```

**Parallel builds:**
```bash
# Make (use all cores)
make -j$(nproc)

# Ninja (automatically parallel)
ninja
```

## Verification

After building, verify the installation:

```bash
# Check version
./yaqeen --version

# Run tests
ctest

# Test template creation
./yaqeen create --template react-typescript --name test --dry-run
```

## Next Steps

- Read the [Contributing Guide](CONTRIBUTING.md)
- Check out [Development Documentation](docs/development/architecture.md)
- Join our community and share your experience!
