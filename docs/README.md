# Yaqeen Documentation

Welcome to the comprehensive documentation for **Yaqeen** - a lightning-fast, production-ready CLI tool for generating project structures from markdown files or pre-built templates.

## What is Yaqeen?

Yaqeen (يقين - Arabic for "certainty") is a high-performance project scaffolding tool written in C++17 that helps developers quickly create well-structured projects following industry best practices.

### Key Features

- ⚡ **Lightning Fast** - Optimized C++ implementation generates projects in < 200ms
- 🎨 **Beautiful UI** - Tokyo Night themed terminal interface with smooth animations
- 📦 **30+ Templates** - Pre-built templates for popular frameworks and architectures
- 📝 **Markdown Support** - Define structures in simple markdown files
- 🔧 **Highly Configurable** - Extensive configuration options and profiles
- 🚀 **Production Ready** - Comprehensive error handling, logging, and testing
- 🌐 **Cross-Platform** - Works on Linux, macOS, and Windows
- 🔌 **Extensible** - Easy to create custom templates and integrate as a library

## Quick Start

### Installation

```bash
# Linux/macOS
wget https://github.com/yourusername/yaqeen/releases/latest/download/yaqeen-linux-x64
chmod +x yaqeen-linux-x64
sudo mv yaqeen-linux-x64 /usr/local/bin/yaqeen

# From source
git clone https://github.com/yourusername/yaqeen.git
cd yaqeen && mkdir build && cd build
cmake .. && make && sudo make install
```

### Create Your First Project

```bash
# Create from template
yaqeen create --template react-typescript --name my-app

# Create from markdown
yaqeen init structure.md

# Interactive mode
yaqeen create --interactive
```

## Documentation Structure

### 📚 Getting Started
- [Installation](getting-started/installation.md) - Install Yaqeen on your system
- [Quick Start](getting-started/quick-start.md) - Get up and running in minutes
- [Your First Project](getting-started/first-project.md) - Step-by-step guide

### 🔨 Usage
- [CLI Reference](usage/cli-reference.md) - Complete CLI documentation
- [Markdown Format](usage/markdown-format.md) - Markdown specification
- [Using Templates](usage/templates.md) - Work with templates
- [Interactive Mode](usage/interactive-mode.md) - Beautiful terminal UI

### 📋 Templates
- [Overview](templates/overview.md) - All available templates
- [Custom Templates](templates/custom-templates.md) - Create your own

### ⚙️ Advanced
- [Configuration](advanced/configuration.md) - Configure Yaqeen
- [CI/CD Integration](advanced/ci-cd.md) - GitHub Actions, GitLab CI, Jenkins

### 🛠️ Development
- [Building from Source](development/building.md) - Compile from source
- [Architecture](development/architecture.md) - Internal design
- [Contributing](development/contributing.md) - Contribute to Yaqeen

### 💻 API
- [Library Usage](api/library-usage.md) - Use Yaqeen in C++ projects
- Complete API reference and examples

## Performance

- **Small projects** (< 50 files): < 50ms
- **Medium projects** (50-200 files): 50-150ms
- **Large projects** (200-1000 files): 150-500ms

## Support

- **📖 Documentation**: You're reading it!
- **🐛 Issues**: [GitHub Issues](https://github.com/yourusername/yaqeen/issues)
- **💬 Discussions**: [GitHub Discussions](https://github.com/yourusername/yaqeen/discussions)

## License

Yaqeen is open source software licensed under the [MIT License](../LICENSE).

---

**Ready to get started?** Head to [Installation](getting-started/installation.md) or [Quick Start](getting-started/quick-start.md)!
