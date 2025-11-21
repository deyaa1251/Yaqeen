# Creating Custom Templates

This guide explains how to create, test, and distribute custom templates for Yaqeen. Learn the template JSON schema, best practices, and advanced features.

## Template Anatomy

### Basic Structure

A Yaqeen template is a JSON file with the following schema:

```json
{
  "name": "template-name",
  "description": "Brief description of the template",
  "version": "1.0.0",
  "category": "web|backend|mobile|patterns|languages|other",
  "tags": ["tag1", "tag2", "tag3"],
  "author": "Your Name <email@example.com>",
  "license": "MIT",
  "structure": {
    "src/": {
      "main.ts": "",
      "utils/": {}
    },
    "README.md": ""
  }
}
```

### Schema Fields

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `name` | string | ✅ | Unique template identifier (kebab-case) |
| `description` | string | ✅ | Short description (< 100 chars) |
| `version` | string | ✅ | Semantic version (major.minor.patch) |
| `category` | string | ✅ | Template category |
| `tags` | array | ❌ | Searchable keywords |
| `author` | string | ❌ | Author information |
| `license` | string | ❌ | License identifier |
| `structure` | object | ✅ | Project structure definition |

### Structure Definition

The `structure` object defines the project hierarchy:

**Directories:** Keys ending with `/` or with object values
```json
"src/": {}              // Empty directory
"lib/": {               // Directory with contents
  "utils.ts": ""
}
```

**Files:** Keys with string values
```json
"README.md": ""                 // Empty file
"package.json": "{ ... }"       // File with content
```

**Nesting:** Objects within objects
```json
"src/": {
  "core/": {
    "engine.ts": "",
    "config.ts": ""
  }
}
```

## Creating Your First Template

### Step 1: Design the Structure

Plan your project structure:

```
my-template/
├── src/
│   ├── index.ts
│   ├── core/
│   └── utils/
├── tests/
├── docs/
├── package.json
├── tsconfig.json
└── README.md
```

### Step 2: Write the JSON

Create `my-template.json`:

```json
{
  "name": "my-typescript-template",
  "description": "TypeScript project with modern tooling",
  "version": "1.0.0",
  "category": "languages",
  "tags": ["typescript", "nodejs", "testing"],
  "author": "Your Name",
  "structure": {
    "src/": {
      "index.ts": "",
      "core/": {},
      "utils/": {}
    },
    "tests/": {},
    "docs/": {},
    "package.json": "",
    "tsconfig.json": "",
    "README.md": ""
  }
}
```

### Step 3: Add File Content

Templates can include file contents:

```json
{
  "name": "my-template",
  "description": "Template with file contents",
  "version": "1.0.0",
  "category": "other",
  "structure": {
    "src/": {
      "index.ts": "console.log('Hello from template!');\n"
    },
    "package.json": "{\n  \"name\": \"{{project_name}}\",\n  \"version\": \"1.0.0\"\n}",
    "README.md": "# {{project_name}}\n\nGenerated with Yaqeen.\n"
  }
}
```

**Template variables:**
- `{{project_name}}` - Project name
- `{{author}}` - Author name (from git config)
- `{{date}}` - Current date
- `{{year}}` - Current year

### Step 4: Validate the Template

Use Yaqeen to validate your template:

```bash
yaqeen template validate my-template.json
```

Output:
```
✓ Template structure is valid
✓ All required fields present
✓ Structure tree is well-formed
✓ No syntax errors

Template: my-typescript-template v1.0.0
Files: 5
Directories: 4
```

### Step 5: Test the Template

Create a test project:

```bash
yaqeen create --template ./my-template.json --name test-project --dry-run
```

Review the output, then create for real:

```bash
yaqeen create --template ./my-template.json --name test-project
cd test-project
ls -la
```

### Step 6: Install the Template

Install to user templates directory:

```bash
yaqeen template install my-template.json
```

Template is now available:

```bash
yaqeen list
# Shows: my-typescript-template

yaqeen create --template my-typescript-template --name project
```

## Advanced Features

### File Content Templates

Use advanced templating for dynamic content:

```json
{
  "structure": {
    "src/": {
      "config.ts": "export const config = {\n  name: '{{project_name}}',\n  version: '1.0.0',\n  author: '{{author}}',\n  created: '{{date}}'\n};\n"
    },
    "README.md": "# {{project_name}}\n\nCreated: {{date}}\nAuthor: {{author}}\n\n## Installation\n\n```bash\nnpm install\n```\n"
  }
}
```

Variables available:
```javascript
{
  project_name: "my-app",           // From --name flag
  author: "John Doe",               // From git config user.name
  email: "john@example.com",        // From git config user.email
  date: "2024-11-19",              // Current date (YYYY-MM-DD)
  year: "2024",                     // Current year
  datetime: "2024-11-19 10:30:45"  // Full timestamp
}
```

### Conditional Files

Include files based on conditions:

```json
{
  "structure": {
    "src/": {
      "index.ts": ""
    },
    "conditional": {
      "typescript": {
        "tsconfig.json": "",
        "tsconfig.build.json": ""
      },
      "testing": {
        "tests/": {
          "setup.ts": "",
          "helpers.ts": ""
        }
      }
    }
  },
  "options": {
    "typescript": {
      "type": "boolean",
      "default": true,
      "description": "Include TypeScript configuration"
    },
    "testing": {
      "type": "boolean",
      "default": false,
      "description": "Include testing setup"
    }
  }
}
```

Usage:
```bash
yaqeen create --template my-template --name app --typescript --testing
```

### Multi-Variant Templates

Create templates with variants:

```json
{
  "name": "react-template",
  "description": "React template with multiple variants",
  "version": "1.0.0",
  "category": "web",
  "variants": {
    "basic": {
      "description": "Basic React setup",
      "structure": {
        "src/": {
          "App.jsx": "",
          "main.jsx": ""
        }
      }
    },
    "typescript": {
      "description": "React with TypeScript",
      "structure": {
        "src/": {
          "App.tsx": "",
          "main.tsx": ""
        },
        "tsconfig.json": ""
      }
    },
    "full": {
      "description": "Full-featured React setup",
      "extends": "typescript",
      "structure": {
        "src/": {
          "components/": {},
          "hooks/": {},
          "utils/": {}
        },
        "tests/": {}
      }
    }
  },
  "default_variant": "basic"
}
```

Usage:
```bash
yaqeen create --template react-template --variant typescript --name app
```

### Post-Generation Scripts

Run scripts after project creation:

```json
{
  "name": "my-template",
  "description": "Template with post-generation hooks",
  "version": "1.0.0",
  "category": "other",
  "structure": {
    "src/": {}
  },
  "hooks": {
    "post_generate": [
      "npm install",
      "git init",
      "git add .",
      "git commit -m 'Initial commit'"
    ]
  },
  "options": {
    "auto_git": {
      "type": "boolean",
      "default": false,
      "description": "Initialize git repository"
    }
  }
}
```

### Template Dependencies

Specify dependencies between templates:

```json
{
  "name": "fullstack-template",
  "description": "Full-stack application",
  "version": "1.0.0",
  "category": "other",
  "dependencies": {
    "backend": "express",
    "frontend": "react-typescript"
  },
  "structure": {
    "packages/": {
      "backend/": {
        "_template": "express"
      },
      "frontend/": {
        "_template": "react-typescript"
      }
    },
    "README.md": ""
  }
}
```

## Complex Example

### Enterprise Backend Template

```json
{
  "name": "enterprise-backend",
  "description": "Enterprise-grade backend with clean architecture",
  "version": "2.0.0",
  "category": "patterns",
  "tags": ["enterprise", "clean-architecture", "typescript", "nodejs"],
  "author": "Your Company <tech@company.com>",
  "license": "MIT",
  "structure": {
    "src/": {
      "domain/": {
        "entities/": {
          "User.ts": "",
          "Product.ts": ""
        },
        "repositories/": {
          "IUserRepository.ts": "",
          "IProductRepository.ts": ""
        },
        "usecases/": {
          "CreateUser.ts": "",
          "GetUser.ts": ""
        }
      },
      "application/": {
        "services/": {
          "UserService.ts": "",
          "ProductService.ts": ""
        },
        "dto/": {
          "UserDTO.ts": "",
          "ProductDTO.ts": ""
        }
      },
      "infrastructure/": {
        "database/": {
          "PrismaUserRepository.ts": "",
          "migrations/": {}
        },
        "cache/": {
          "RedisCache.ts": ""
        },
        "queue/": {
          "BullQueue.ts": ""
        }
      },
      "presentation/": {
        "http/": {
          "controllers/": {
            "UserController.ts": "",
            "ProductController.ts": ""
          },
          "middleware/": {
            "auth.ts": "",
            "validation.ts": "",
            "errorHandler.ts": ""
          },
          "routes/": {
            "index.ts": "",
            "users.ts": "",
            "products.ts": ""
          }
        }
      },
      "shared/": {
        "utils/": {
          "logger.ts": "",
          "validator.ts": ""
        },
        "types/": {
          "index.ts": ""
        },
        "constants/": {
          "errors.ts": "",
          "config.ts": ""
        }
      },
      "index.ts": "// Application entry point\nimport { startServer } from './presentation/http/server';\n\nstartServer();\n"
    },
    "tests/": {
      "unit/": {
        "domain/": {},
        "application/": {}
      },
      "integration/": {
        "api/": {}
      },
      "e2e/": {}
    },
    "config/": {
      "default.json": "",
      "production.json": "",
      "development.json": ""
    },
    "scripts/": {
      "seed.ts": "",
      "migrate.ts": ""
    },
    "docs/": {
      "API.md": "",
      "ARCHITECTURE.md": "",
      "DEVELOPMENT.md": ""
    },
    ".github/": {
      "workflows/": {
        "ci.yml": "",
        "cd.yml": ""
      }
    },
    "package.json": "{\n  \"name\": \"{{project_name}}\",\n  \"version\": \"1.0.0\",\n  \"description\": \"Enterprise backend application\",\n  \"main\": \"dist/index.js\",\n  \"scripts\": {\n    \"dev\": \"ts-node-dev src/index.ts\",\n    \"build\": \"tsc\",\n    \"start\": \"node dist/index.js\",\n    \"test\": \"jest\",\n    \"lint\": \"eslint src/**/*.ts\"\n  }\n}\n",
    "tsconfig.json": "",
    "jest.config.js": "",
    ".eslintrc.json": "",
    ".prettierrc": "",
    ".env.example": "",
    ".gitignore": "",
    "README.md": "# {{project_name}}\n\nEnterprise backend application.\n\n## Architecture\n\nThis project follows Clean Architecture principles.\n\n## Getting Started\n\n```bash\nnpm install\nnpm run dev\n```\n",
    "Dockerfile": "",
    "docker-compose.yml": ""
  },
  "options": {
    "database": {
      "type": "select",
      "choices": ["postgresql", "mongodb", "mysql"],
      "default": "postgresql",
      "description": "Database system to use"
    },
    "cache": {
      "type": "boolean",
      "default": true,
      "description": "Include Redis caching"
    },
    "queue": {
      "type": "boolean",
      "default": false,
      "description": "Include job queue (Bull)"
    }
  },
  "hooks": {
    "post_generate": [
      "npm install",
      "npx prisma init"
    ]
  }
}
```

## Testing Templates

### Unit Testing

Test template structure programmatically:

```cpp
#include <yaqeen/core/template_manager.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Template loads successfully", "[template]") {
    TemplateManager manager;
    auto result = manager.load_template("my-template.json");

    REQUIRE(result.is_ok());

    auto tmpl = result.value();
    REQUIRE(tmpl.name == "my-template");
    REQUIRE(tmpl.version == "1.0.0");
    REQUIRE(tmpl.structure.children.size() > 0);
}

TEST_CASE("Template generates correct structure", "[template]") {
    TemplateManager manager;
    auto tmpl = manager.load_template("my-template.json").value();

    Generator generator;
    auto result = generator.generate(tmpl, "test-output");

    REQUIRE(result.is_ok());
    REQUIRE(fs::exists("test-output/src"));
    REQUIRE(fs::exists("test-output/README.md"));
}
```

### Integration Testing

Test template end-to-end:

```bash
#!/bin/bash

# Test script for templates

TEMPLATE="my-template.json"
TEST_DIR="test-output"

# Clean up
rm -rf "$TEST_DIR"

# Create project
yaqeen create --template "$TEMPLATE" --name "$TEST_DIR" || exit 1

# Verify structure
test -d "$TEST_DIR/src" || { echo "Missing src/"; exit 1; }
test -f "$TEST_DIR/README.md" || { echo "Missing README.md"; exit 1; }

# Verify content
grep "{{project_name}}" "$TEST_DIR/README.md" && { echo "Unresolved variable"; exit 1; }

# Try building
cd "$TEST_DIR"
npm install || exit 1
npm run build || exit 1

echo "✓ Template test passed"
```

## Best Practices

### 1. Naming Conventions

**Template names:**
- Use kebab-case: `my-awesome-template`
- Be descriptive: `react-typescript-vite` not `react`
- Include key technologies: `nextjs-tailwind-prisma`

**File names:**
- Follow framework conventions
- Use consistent casing (camelCase, snake_case, PascalCase)

### 2. Structure Organization

**Group related files:**
```json
{
  "src/": {
    "features/": {          // Feature-based organization
      "auth/": {},
      "users/": {},
      "products/": {}
    }
  }
}
```

**Separate concerns:**
```json
{
  "src/": {},              // Source code
  "tests/": {},            // Tests
  "docs/": {},             // Documentation
  "scripts/": {},          // Utility scripts
  "config/": {}            // Configuration
}
```

### 3. Documentation

Include comprehensive README:

```json
{
  "structure": {
    "README.md": "# {{project_name}}\n\n## Quick Start\n\n```bash\nnpm install\nnpm run dev\n```\n\n## Project Structure\n\n- `src/` - Source code\n- `tests/` - Tests\n- `docs/` - Documentation\n\n## Scripts\n\n- `npm run dev` - Development server\n- `npm run build` - Production build\n- `npm test` - Run tests\n\n## License\n\nMIT\n"
  }
}
```

### 4. Version Control

Include appropriate `.gitignore`:

```json
{
  "structure": {
    ".gitignore": "node_modules/\ndist/\nbuild/\n.env\n*.log\n.DS_Store\n"
  }
}
```

### 5. Configuration Files

Include essential configs:

```json
{
  "structure": {
    "tsconfig.json": "{...}",
    ".eslintrc.json": "{...}",
    ".prettierrc": "{...}",
    "jest.config.js": "{...}"
  }
}
```

### 6. Semantic Versioning

Follow semver:
- **Major (1.0.0)**: Breaking changes
- **Minor (0.1.0)**: New features
- **Patch (0.0.1)**: Bug fixes

## Distribution

### Publishing Templates

**Option 1: Local Installation**
```bash
yaqeen template install my-template.json
```

**Option 2: Git Repository**
```bash
# Users can install from Git
yaqeen template install https://github.com/user/template-repo/my-template.json
```

**Option 3: NPM Package**
```bash
# Package template as NPM module
npm publish yaqeen-template-mytemplate

# Users install via NPM
npm install -g yaqeen-template-mytemplate
yaqeen template link yaqeen-template-mytemplate
```

**Option 4: Template Registry**
```bash
# Submit to Yaqeen template registry
yaqeen template publish my-template.json --registry official
```

### Template Marketplace

Submit templates to Yaqeen Marketplace:

1. **Fork registry repository**
2. **Add template JSON**
3. **Update catalog**
4. **Submit pull request**
5. **Template reviewed and published**

## Maintenance

### Updating Templates

```bash
# Update template version
yaqeen template update my-template --version 1.1.0

# Publish update
yaqeen template publish my-template.json
```

### Deprecating Templates

```json
{
  "name": "old-template",
  "version": "1.0.0",
  "deprecated": true,
  "deprecation_message": "Use 'new-template' instead",
  "replacement": "new-template"
}
```

## Summary

Key points:
- ✅ Templates are JSON files defining project structure
- ✅ Include metadata, structure, and optional file content
- ✅ Support template variables and conditional files
- ✅ Test thoroughly before distribution
- ✅ Follow naming and organization best practices
- ✅ Document usage and structure
- ✅ Use semantic versioning

Next steps:
- Create your first template
- Test with `yaqeen template validate`
- Share with the community
- Explore [Template Development](../development/templates.md)
