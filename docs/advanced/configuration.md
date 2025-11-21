# Configuration

Yaqeen supports multiple configuration methods to customize behavior, set defaults, and optimize workflows. This guide covers all configuration options and best practices.

## Configuration Hierarchy

Yaqeen loads configuration from multiple sources in order of precedence:

1. **Command-line flags** (highest priority)
2. **Environment variables**
3. **Project configuration** (`.yaqeen.json` in project root)
4. **User configuration** (`~/.yaqeen/config.json`)
5. **System configuration** (`/etc/yaqeen/config.json`)
6. **Default values** (lowest priority)

## Configuration File Format

### Basic Structure

```json
{
  "version": "1.0",
  "defaults": {
    "output_dir": "./projects",
    "templates_dir": "~/.yaqeen/templates",
    "dry_run": false,
    "verbose": false,
    "force": false
  },
  "templates": {
    "search_paths": [
      "~/.yaqeen/templates",
      "/usr/local/share/yaqeen/templates",
      "./templates"
    ],
    "auto_update": false
  },
  "ui": {
    "theme": "tokyo-night",
    "animations": true,
    "vim_keys": true,
    "color": "auto"
  },
  "logging": {
    "level": "info",
    "file": null,
    "format": "pretty"
  },
  "performance": {
    "parallel_creation": true,
    "max_threads": 4,
    "buffer_size": 8192
  }
}
```

## Configuration Options

### General Settings

#### `defaults.output_dir`
**Type:** `string`
**Default:** `"."`
**Description:** Default output directory for created projects

```json
{
  "defaults": {
    "output_dir": "~/projects"
  }
}
```

Usage:
```bash
# Uses ~/projects as base
yaqeen create --template react --name my-app
# Creates ~/projects/my-app
```

#### `defaults.templates_dir`
**Type:** `string`
**Default:** `"~/.yaqeen/templates"`
**Description:** Primary template search directory

```json
{
  "defaults": {
    "templates_dir": "/path/to/custom/templates"
  }
}
```

#### `defaults.dry_run`
**Type:** `boolean`
**Default:** `false`
**Description:** Default dry-run mode

```json
{
  "defaults": {
    "dry_run": true  // Always preview first
  }
}
```

#### `defaults.verbose`
**Type:** `boolean`
**Default:** `false`
**Description:** Enable verbose logging by default

```json
{
  "defaults": {
    "verbose": true  // Always show detailed output
  }
}
```

### Template Configuration

#### `templates.search_paths`
**Type:** `array<string>`
**Default:** `["~/.yaqeen/templates", "/usr/local/share/yaqeen/templates"]`
**Description:** Directories to search for templates (in order)

```json
{
  "templates": {
    "search_paths": [
      "~/my-templates",           // Search here first
      "~/.yaqeen/templates",      // Then user directory
      "/usr/share/yaqeen/templates"  // Then system directory
    ]
  }
}
```

#### `templates.auto_update`
**Type:** `boolean`
**Default:** `false`
**Description:** Automatically check for template updates

```json
{
  "templates": {
    "auto_update": true,
    "update_interval": "daily"  // daily, weekly, monthly
  }
}
```

#### `templates.aliases`
**Type:** `object`
**Description:** Create template aliases/shortcuts

```json
{
  "templates": {
    "aliases": {
      "react": "react-typescript",
      "vue": "vue3",
      "api": "nestjs",
      "web": "nextjs"
    }
  }
}
```

Usage:
```bash
yaqeen create --template react --name app
# Uses react-typescript template
```

### UI Configuration

#### `ui.theme`
**Type:** `string`
**Default:** `"tokyo-night"`
**Options:** `tokyo-night`, `dracula`, `nord`, `solarized-dark`, `solarized-light`, `monokai`
**Description:** Interactive mode color theme

```json
{
  "ui": {
    "theme": "nord"
  }
}
```

#### `ui.animations`
**Type:** `boolean`
**Default:** `true`
**Description:** Enable UI animations

```json
{
  "ui": {
    "animations": false  // Disable for better performance
  }
}
```

#### `ui.vim_keys`
**Type:** `boolean`
**Default:** `true`
**Description:** Enable Vim-style navigation (hjkl)

```json
{
  "ui": {
    "vim_keys": true
  }
}
```

#### `ui.color`
**Type:** `string`
**Default:** `"auto"`
**Options:** `auto`, `always`, `never`
**Description:** Color output control

```json
{
  "ui": {
    "color": "always"  // Force color even when piping
  }
}
```

### Logging Configuration

#### `logging.level`
**Type:** `string`
**Default:** `"info"`
**Options:** `debug`, `info`, `warning`, `error`
**Description:** Logging verbosity level

```json
{
  "logging": {
    "level": "debug"  // Show all logs
  }
}
```

#### `logging.file`
**Type:** `string | null`
**Default:** `null`
**Description:** Log file path

```json
{
  "logging": {
    "file": "~/.yaqeen/logs/yaqeen.log"
  }
}
```

#### `logging.format`
**Type:** `string`
**Default:** `"pretty"`
**Options:** `pretty`, `json`, `compact`
**Description:** Log output format

```json
{
  "logging": {
    "format": "json"  // Machine-readable logs
  }
}
```

Example JSON log:
```json
{
  "timestamp": "2024-11-19T10:30:45Z",
  "level": "info",
  "message": "Project created successfully",
  "context": {
    "template": "react-typescript",
    "files": 18,
    "time_ms": 127
  }
}
```

### Performance Configuration

#### `performance.parallel_creation`
**Type:** `boolean`
**Default:** `true`
**Description:** Create files in parallel

```json
{
  "performance": {
    "parallel_creation": true
  }
}
```

#### `performance.max_threads`
**Type:** `integer`
**Default:** `4`
**Range:** `1-16`
**Description:** Maximum parallel threads

```json
{
  "performance": {
    "max_threads": 8  // Use more threads on powerful machines
  }
}
```

#### `performance.buffer_size`
**Type:** `integer`
**Default:** `8192`
**Description:** I/O buffer size in bytes

```json
{
  "performance": {
    "buffer_size": 16384  // Larger buffer for better I/O performance
  }
}
```

## Environment Variables

Override configuration with environment variables:

### Core Variables

```bash
# Template directory
export YAQEEN_TEMPLATES_DIR=/path/to/templates

# Default output directory
export YAQEEN_OUTPUT_DIR=~/projects

# Logging level
export YAQEEN_LOG_LEVEL=debug

# Log file
export YAQEEN_LOG_FILE=~/.yaqeen/yaqeen.log

# Disable colors
export YAQEEN_COLOR=never

# Config file location
export YAQEEN_CONFIG=~/custom-config.json
```

### Template Variables

```bash
# Author name (used in templates)
export YAQEEN_AUTHOR="John Doe"

# Author email
export YAQEEN_EMAIL="john@example.com"

# License
export YAQEEN_LICENSE="MIT"
```

### Performance Variables

```bash
# Max threads
export YAQEEN_MAX_THREADS=8

# Enable parallel creation
export YAQEEN_PARALLEL=true
```

## Project Configuration

Create `.yaqeen.json` in project root for project-specific settings:

```json
{
  "version": "1.0",
  "defaults": {
    "templates_dir": "./templates",
    "output_dir": "./packages"
  },
  "templates": {
    "aliases": {
      "service": "./templates/microservice.json",
      "lib": "./templates/library.json"
    }
  }
}
```

Usage:
```bash
cd my-monorepo
yaqeen create --template service --name auth-service
# Uses ./templates/microservice.json
# Creates ./packages/auth-service
```

## Configuration Management

### View Configuration

```bash
# Show current configuration
yaqeen config show

# Show specific value
yaqeen config get defaults.output_dir
yaqeen config get ui.theme
```

### Set Configuration

```bash
# Set value
yaqeen config set defaults.output_dir ~/projects
yaqeen config set ui.theme nord
yaqeen config set logging.level debug

# Set nested value
yaqeen config set templates.search_paths[0] ~/my-templates
```

### Reset Configuration

```bash
# Reset to defaults
yaqeen config reset

# Reset specific value
yaqeen config reset defaults.output_dir
```

### Export/Import Configuration

```bash
# Export configuration
yaqeen config export > my-config.json

# Import configuration
yaqeen config import my-config.json
```

## Advanced Configuration

### Custom Templates Per Project Type

```json
{
  "templates": {
    "search_paths": [
      "~/templates/${project_type}",
      "~/.yaqeen/templates"
    ]
  }
}
```

Usage:
```bash
export PROJECT_TYPE=microservices
yaqeen create --template service --name my-service
# Searches ~/templates/microservices first
```

### Conditional Configuration

```json
{
  "defaults": {
    "output_dir": "${HOME}/projects",
    "templates_dir": "${HOME}/.yaqeen/templates"
  },
  "environments": {
    "development": {
      "defaults": {
        "verbose": true,
        "dry_run": false
      }
    },
    "production": {
      "defaults": {
        "verbose": false,
        "dry_run": true
      }
    }
  }
}
```

Usage:
```bash
export YAQEEN_ENV=production
yaqeen create --template react --name app
# Uses production settings (dry_run=true)
```

### Git Integration

```json
{
  "git": {
    "auto_init": true,
    "initial_commit": true,
    "commit_message": "chore: initial commit from yaqeen",
    "default_branch": "main",
    "remote": {
      "auto_add": false,
      "url_template": "git@github.com:${author}/${project_name}.git"
    }
  }
}
```

### Hook Configuration

```json
{
  "hooks": {
    "pre_create": [
      "echo 'Creating project...'"
    ],
    "post_create": [
      "cd ${output_dir} && npm install",
      "cd ${output_dir} && git init",
      "cd ${output_dir} && code ."
    ],
    "on_error": [
      "notify-send 'Yaqeen' 'Project creation failed'"
    ]
  }
}
```

## Configuration Profiles

Create multiple configuration profiles:

### `~/.yaqeen/profiles/work.json`
```json
{
  "defaults": {
    "output_dir": "~/work/projects"
  },
  "templates": {
    "aliases": {
      "api": "enterprise-backend",
      "web": "nextjs-enterprise"
    }
  },
  "git": {
    "auto_init": true,
    "remote": {
      "url_template": "git@gitlab.company.com:${author}/${project_name}.git"
    }
  }
}
```

### `~/.yaqeen/profiles/personal.json`
```json
{
  "defaults": {
    "output_dir": "~/personal/projects"
  },
  "templates": {
    "aliases": {
      "api": "fastapi",
      "web": "react-typescript"
    }
  },
  "git": {
    "auto_init": true,
    "remote": {
      "url_template": "git@github.com:${author}/${project_name}.git"
    }
  }
}
```

Usage:
```bash
# Use work profile
yaqeen --profile work create --template api --name project

# Or set default profile
yaqeen config set default_profile work
```

## Configuration Schema

Full configuration schema for validation:

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "type": "object",
  "properties": {
    "version": {
      "type": "string",
      "pattern": "^\\d+\\.\\d+$"
    },
    "defaults": {
      "type": "object",
      "properties": {
        "output_dir": { "type": "string" },
        "templates_dir": { "type": "string" },
        "dry_run": { "type": "boolean" },
        "verbose": { "type": "boolean" },
        "force": { "type": "boolean" }
      }
    },
    "templates": {
      "type": "object",
      "properties": {
        "search_paths": {
          "type": "array",
          "items": { "type": "string" }
        },
        "auto_update": { "type": "boolean" },
        "aliases": {
          "type": "object",
          "additionalProperties": { "type": "string" }
        }
      }
    },
    "ui": {
      "type": "object",
      "properties": {
        "theme": {
          "type": "string",
          "enum": ["tokyo-night", "dracula", "nord", "solarized-dark", "solarized-light", "monokai"]
        },
        "animations": { "type": "boolean" },
        "vim_keys": { "type": "boolean" },
        "color": {
          "type": "string",
          "enum": ["auto", "always", "never"]
        }
      }
    },
    "logging": {
      "type": "object",
      "properties": {
        "level": {
          "type": "string",
          "enum": ["debug", "info", "warning", "error"]
        },
        "file": {
          "type": ["string", "null"]
        },
        "format": {
          "type": "string",
          "enum": ["pretty", "json", "compact"]
        }
      }
    },
    "performance": {
      "type": "object",
      "properties": {
        "parallel_creation": { "type": "boolean" },
        "max_threads": {
          "type": "integer",
          "minimum": 1,
          "maximum": 16
        },
        "buffer_size": {
          "type": "integer",
          "minimum": 1024
        }
      }
    }
  }
}
```

## Best Practices

### 1. Use Project Configuration

For monorepos or team projects:

```json
// .yaqeen.json
{
  "defaults": {
    "templates_dir": "./templates",
    "output_dir": "./packages"
  }
}
```

Commit to version control so team shares configuration.

### 2. Environment-Specific Settings

```bash
# .env.development
export YAQEEN_LOG_LEVEL=debug
export YAQEEN_VERBOSE=true

# .env.production
export YAQEEN_LOG_LEVEL=error
export YAQEEN_VERBOSE=false
```

### 3. Use Configuration Profiles

Create profiles for different contexts (work, personal, client projects).

### 4. Document Custom Configuration

```markdown
# Project Setup

1. Install Yaqeen
2. Copy `.yaqeen.json` to project root
3. Run: `yaqeen create --template service --name my-service`
```

### 5. Validate Configuration

```bash
# Validate configuration file
yaqeen config validate ~/.yaqeen/config.json
```

## Troubleshooting

### Configuration Not Loading

**Check file locations:**
```bash
yaqeen config paths
# Shows:
# User config: ~/.yaqeen/config.json
# Project config: ./.yaqeen.json (not found)
# System config: /etc/yaqeen/config.json (not found)
```

### Invalid JSON

**Validate JSON:**
```bash
cat ~/.yaqeen/config.json | jq .
# If invalid, shows error
```

### Permission Issues

**Check file permissions:**
```bash
ls -la ~/.yaqeen/config.json
chmod 644 ~/.yaqeen/config.json
```

## Summary

Key takeaways:
- ✅ Configuration hierarchy: CLI > env > project > user > system > defaults
- ✅ Use `~/.yaqeen/config.json` for user settings
- ✅ Use `.yaqeen.json` for project settings
- ✅ Environment variables override file configuration
- ✅ Create profiles for different workflows
- ✅ Use `yaqeen config` commands to manage settings

Next steps:
- Create your configuration file
- Set up profiles
- Explore [Scripting](scripting.md)
- Check [CI/CD Integration](ci-cd.md)
