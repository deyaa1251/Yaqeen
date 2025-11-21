# CLI Reference

Complete reference for all Yaqeen commands and options.

## Global Options

These options can be used with any command:

| Option | Description |
|--------|-------------|
| `-v, --verbose` | Enable verbose output with detailed logging |
| `--dry-run` | Preview changes without creating files |
| `--log-file <path>` | Write logs to specified file |
| `--templates-dir <path>` | Use custom templates directory |
| `--help` | Display help information |
| `--version` | Display version information |

## Commands

### `init`

Initialize a project from a markdown file.

**Usage:**
```bash
yaqeen init <file> [options]
```

**Arguments:**
- `<file>` - Path to markdown file containing project structure

**Options:**
- `-o, --output <directory>` - Output directory (default: current directory)

**Examples:**
```bash
# Create in current directory
yaqeen init structure.md

# Create in specific directory
yaqeen init structure.md --output ./projects/myapp

# Preview without creating
yaqeen init structure.md --dry-run

# Verbose output
yaqeen init structure.md --verbose
```

### `create`

Create a project from a template.

**Usage:**
```bash
yaqeen create --template <name> --name <project> [options]
```

**Options:**
- `-t, --template <name>` - Template name (required)
- `-n, --name <project>` - Project name (required)
- `-o, --output <directory>` - Output directory (default: project name)

**Examples:**
```bash
# Create React project
yaqeen create --template react-typescript --name my-app

# Create with custom output
yaqeen create -t laravel -n blog --output ./sites/blog

# Preview creation
yaqeen create -t django -n api --dry-run
```

### `list`

List available templates.

**Usage:**
```bash
yaqeen list [options]
```

**Options:**
- `-c, --category <category>` - Filter by category

**Examples:**
```bash
# List all templates
yaqeen list

# List web templates
yaqeen list --category web

# List backend templates
yaqeen list --category backend
```

**Categories:**
- `web` - Frontend web frameworks
- `backend` - Backend frameworks
- `mobile` - Mobile development
- `patterns` - Architecture patterns
- `languages` - Language-specific projects
- `other` - Miscellaneous templates

### `show`

Show detailed information about a template.

**Usage:**
```bash
yaqeen show <template>
```

**Arguments:**
- `<template>` - Template name

**Examples:**
```bash
# Show React template details
yaqeen show react-typescript

# Show Clean Architecture details
yaqeen show clean-architecture
```

## Exit Codes

| Code | Description |
|------|-------------|
| 0 | Success |
| 1 | General error (file not found, invalid input, etc.) |
| 2 | Template not found |
| 3 | Permission denied |

## Environment Variables

| Variable | Description |
|----------|-------------|
| `YAQEEN_TEMPLATES_DIR` | Default templates directory |
| `YAQEEN_LOG_LEVEL` | Log level (DEBUG, INFO, WARN, ERROR) |

**Example:**
```bash
export YAQEEN_TEMPLATES_DIR=/custom/templates
export YAQEEN_LOG_LEVEL=DEBUG
yaqeen create --template my-custom --name myapp
```

## Configuration File

Yaqeen looks for configuration in these locations (in order):

1. `.yaqeen.json` in current directory
2. `~/.config/yaqeen/config.json`
3. `/etc/yaqeen/config.json`

**Example configuration:**

```json
{
  "templatesDir": "/custom/templates",
  "defaultCategory": "web",
  "logLevel": "INFO",
  "verbose": false,
  "dryRun": false
}
```

## Shell Completion

### Bash

```bash
# Add to ~/.bashrc
eval "$(yaqeen --completion bash)"
```

### Zsh

```zsh
# Add to ~/.zshrc
eval "$(yaqeen --completion zsh)"
```

### Fish

```fish
# Add to ~/.config/fish/config.fish
yaqeen --completion fish | source
```

## Tips and Tricks

### Alias for Frequent Commands

```bash
# Add to your shell rc file
alias yq='yaqeen'
alias yqc='yaqeen create'
alias yql='yaqeen list'
```

### Quick Project Creation

```bash
# Create and enter directory in one command
yaqeen create -t react-typescript -n myapp && cd myapp
```

### Scripting

```bash
#!/bin/bash
# Create multiple projects
for project in api web admin; do
    yaqeen create -t nextjs -n "$project" -o "./projects/$project"
done
```

### Logging for Debugging

```bash
# Enable debug logging to file
yaqeen create -t laravel -n blog --verbose --log-file debug.log
```

## See Also

- [Templates Overview](../templates/overview.md)
- [Custom Templates](../templates/custom-templates.md)
- [Markdown Format](markdown-format.md)
