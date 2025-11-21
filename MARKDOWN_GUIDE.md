# Yaqeen Markdown Guide

Create custom project structures using simple markdown files!

## Quick Start

1. **Create a markdown file** with your project structure
2. **Run yaqeen init** to generate the structure
3. **Done!** Your project is ready

## Markdown Format

### Basic Structure

````markdown
# My Project Name

Describe your project here...

```
project-root/
├── directory1/
│   ├── file1.txt
│   └── file2.cpp
├── directory2/
│   └── subdirectory/
│       └── file3.hpp
└── README.md
```
````

### Rules

- Use **tree-style** notation with `├──`, `└──`, `│` characters
- Or use **simple indentation** with spaces/tabs
- **Directories** end with `/`
- **Files** don't have trailing `/`
- Nested items are indented

## Examples

### Example 1: Minimal Project

````markdown
# Minimal Project

```
mini/
├── src/
│   └── main.cpp
├── include/
└── README.md
```
````

**Generate it:**
```bash
./yaqeen init minimal.md --output mini-project
```

### Example 2: Full-Stack Application

````markdown
# Full-Stack App

```
fullstack-app/
├── frontend/
│   ├── src/
│   │   ├── App.jsx
│   │   └── main.jsx
│   ├── public/
│   └── package.json
├── backend/
│   ├── src/
│   │   ├── server.js
│   │   └── routes/
│   │       ├── api.js
│   │       └── auth.js
│   ├── config/
│   └── package.json
├── database/
│   ├── migrations/
│   └── seeds/
└── docker-compose.yml
```
````

### Example 3: Monorepo

````markdown
# Monorepo Structure

```
monorepo/
├── packages/
│   ├── core/
│   │   ├── src/
│   │   └── package.json
│   ├── ui/
│   │   ├── src/
│   │   └── package.json
│   └── utils/
│       ├── src/
│       └── package.json
├── apps/
│   ├── web/
│   └── mobile/
├── turbo.json
├── package.json
└── README.md
```
````

## Usage

### Basic Command

```bash
./yaqeen init structure.md
```

This creates the structure in the **current directory**.

### With Custom Output

```bash
./yaqeen init structure.md --output /path/to/project
```

Creates the structure in the specified directory.

### Dry Run (Preview)

```bash
./yaqeen --dry-run init structure.md
```

Shows what would be created without actually creating files.

### Verbose Output

```bash
./yaqeen --verbose init structure.md
```

Shows detailed information during generation.

## Tips & Tricks

### 1. Use Comments

```markdown
# My Project

Here's what each directory does:
- `src/`: Source code
- `tests/`: Unit tests
- `docs/`: Documentation

```
project/
├── src/
├── tests/
└── docs/
```
```

### 2. Mix with Templates

First use a template, then customize:

```bash
# Start with a template
./yaqeen create --template react-typescript --name myapp

# Then add custom structure
./yaqeen init custom-additions.md --output myapp
```

### 3. Version Control Your Structures

Save your markdown files and version control them:

```bash
git add project-structures/
git commit -m "Add project structure templates"
```

### 4. Share Structures

Share markdown files with your team:

```markdown
# Team's Standard API Structure

```
api-project/
├── src/
│   ├── controllers/
│   ├── models/
│   ├── services/
│   ├── middleware/
│   └── app.js
├── tests/
├── config/
└── README.md
```
```

## Common Patterns

### Microservices

```markdown
```
microservices/
├── services/
│   ├── auth-service/
│   ├── user-service/
│   └── payment-service/
├── gateway/
├── shared/
└── docker-compose.yml
```
```

### Documentation Site

```markdown
```
docs-site/
├── docs/
│   ├── getting-started/
│   ├── api/
│   ├── guides/
│   └── index.md
├── static/
└── docusaurus.config.js
```
```

### Library/Package

```markdown
```
my-library/
├── src/
│   ├── index.ts
│   └── core/
├── tests/
├── examples/
├── docs/
├── package.json
├── tsconfig.json
└── README.md
```
```

## Comparison: Markdown vs Templates

| Feature | Markdown | Templates |
|---------|----------|-----------|
| **Flexibility** | ✅ Totally custom | ⚠️ Pre-defined |
| **Speed** | ⚠️ Manual creation | ✅ Instant |
| **Best Practices** | ⚠️ Your choice | ✅ Built-in |
| **Sharing** | ✅ Easy (text file) | ⚠️ JSON config |
| **Complex Structures** | ✅ Any structure | ⚠️ Template limits |

**Best approach:** Use **templates** for standard projects, use **markdown** for custom needs!

## Advanced Usage

### Combining Both

```bash
# 1. Start with template
./yaqeen create --template nestjs --name my-api

# 2. Add custom structure
./yaqeen init custom-modules.md --output my-api
```

### Scripting

```bash
#!/bin/bash
# Generate multiple projects

for project in api web mobile; do
    ./yaqeen init structures/${project}.md --output projects/${project}
done
```

### CI/CD Integration

```yaml
# .github/workflows/setup.yml
- name: Generate project structure
  run: yaqeen init .github/project-structure.md
```

## Troubleshooting

**Q: My nested directories aren't created properly**
A: Make sure to use proper indentation (4 spaces or 1 tab per level)

**Q: Files are created but empty**
A: That's normal! Yaqeen creates the structure. Add content separately.

**Q: Special characters in names?**
A: Stick to alphanumeric, hyphens, underscores, and dots.

**Q: How deep can I nest?**
A: No practical limit! Create as deep as you need.

## Examples to Try

Try these examples in `build/` directory:

```bash
cd /home/omen/yaqeen/build

# Example 1: Simple C++ project
./yaqeen init example-cpp.md --output test-cpp

# Example 2: Web application
./yaqeen init example-web.md --output test-web

# Example 3: Python package
./yaqeen init example-python.md --output test-python
```

## Summary

✅ **Flexible**: Create any structure you want
✅ **Simple**: Just write markdown
✅ **Fast**: Generate instantly
✅ **Shareable**: Text files are easy to share
✅ **Versionable**: Track changes in git

Happy structuring! 🚀
