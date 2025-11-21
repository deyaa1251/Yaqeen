# Your First Project

This guide walks you through creating your first project with Yaqeen, explaining each step in detail and covering important concepts.

## Prerequisites

Before starting, ensure you have:
- Yaqeen installed (see [Installation](installation.md))
- Basic command-line knowledge
- A text editor or IDE

## Understanding Project Templates

Yaqeen uses **templates** - predefined project structures that follow best practices for specific frameworks and architectures. Each template contains:

- **Directory structure** - Folders organized according to framework conventions
- **Boilerplate files** - Essential configuration and starter files
- **Metadata** - Template information (name, description, version, category)

## Creating Your First Project

### Step 1: Explore Available Templates

List all available templates:

```bash
yaqeen list
```

Filter by category:

```bash
yaqeen list --category web
yaqeen list --category backend
yaqeen list --category patterns
```

### Step 2: Get Template Details

Before creating a project, inspect a template to understand what it provides:

```bash
yaqeen show react-typescript
```

Output shows:
```
╔═══════════════════════════════════════════════╗
║ Template: react-typescript                    ║
║ Description: React + TypeScript + Vite        ║
║ Category: web                                 ║
║ Version: 1.0.0                                ║
║ Tags: react, typescript, vite, frontend       ║
╚═══════════════════════════════════════════════╝

Project Structure:
├── src/
│   ├── App.tsx
│   ├── main.tsx
│   ├── components/
│   ├── hooks/
│   ├── utils/
│   └── types/
├── public/
├── tests/
├── package.json
├── tsconfig.json
├── vite.config.ts
└── README.md
```

### Step 3: Create the Project

Create a new project from the template:

```bash
yaqeen create --template react-typescript --name my-app
```

**Command Breakdown:**
- `create` - Command to create a new project
- `--template react-typescript` - Template to use
- `--name my-app` - Name of the project directory

**Optional flags:**
```bash
# Specify custom output directory
yaqeen create --template react-typescript --name my-app --output ./projects/my-app

# Preview without creating (dry run)
yaqeen create --template react-typescript --name my-app --dry-run

# Enable verbose logging
yaqeen create --template react-typescript --name my-app --verbose

# Save logs to file
yaqeen create --template react-typescript --name my-app --log-file creation.log
```

### Step 4: Understand the Output

Yaqeen provides real-time feedback during project creation:

```
  ██    ██  █████   ██████  ███████ ███████ ███    ██
   ██  ██  ██   ██ ██    ██ ██      ██      ████   ██
    ████   ███████ ██    ██ █████   █████   ██ ██  ██
     ██    ██   ██ ██  ▄▄██ ██      ██      ██  ██ ██
     ██    ██   ██  ██████  ███████ ███████ ██   ████

          Project Structure Generator

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

[*] Template: react-typescript v1.0.0
[*] Output: ./my-app
[*] Creating project structure...

  [✓] my-app/
  [✓] my-app/src/
  [✓] my-app/src/App.tsx
  [✓] my-app/src/main.tsx
  [✓] my-app/src/components/
  [✓] my-app/public/
  [✓] my-app/package.json
  [✓] my-app/tsconfig.json
  [✓] my-app/vite.config.ts
  [✓] my-app/README.md

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

[+] Project created successfully!

  Directories: 12
  Files: 28
  Time: 87ms

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Next steps:
  cd my-app
  npm install
  npm run dev
```

**Performance Notes:**
- Yaqeen is written in C++ for maximum performance
- Average project creation: 50-150ms depending on size
- Memory-efficient: minimal heap allocations
- Parallel directory creation when possible

### Step 5: Navigate to Your Project

```bash
cd my-app
```

Examine the structure:

```bash
tree -L 2
# or
ls -la
```

### Step 6: Initialize and Run

Follow framework-specific instructions. For our React example:

```bash
# Install dependencies
npm install

# Start development server
npm run dev
```

Visit `http://localhost:5173` to see your app running.

## Creating from Markdown

Yaqeen's most powerful feature is creating projects from markdown files, allowing you to define custom structures quickly.

### Markdown Structure Syntax

Create a file `myproject.md`:

````markdown
# My Custom C++ Project

This is my project description.

```
myproject/
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── engine.cpp
│   │   ├── engine.hpp
│   │   └── config.cpp
│   ├── utils/
│   │   ├── logger.cpp
│   │   └── logger.hpp
│   └── CMakeLists.txt
├── include/
│   └── myproject/
│       ├── engine.hpp
│       └── types.hpp
├── tests/
│   ├── test_main.cpp
│   ├── test_engine.cpp
│   └── CMakeLists.txt
├── docs/
│   ├── README.md
│   └── API.md
├── CMakeLists.txt
├── README.md
├── LICENSE
└── .gitignore
```
````

**Syntax Rules:**
1. Use a code block (triple backticks) to define the structure
2. Use tree-like syntax with `├──`, `└──`, and `│` characters
3. Directories end with `/`
4. Files have no trailing slash
5. Indentation represents nesting (use spaces or `│   `)

### Generate from Markdown

```bash
yaqeen init myproject.md
```

This creates the entire structure in the current directory.

**Customizing output:**
```bash
# Specify output directory
yaqeen init myproject.md --output ./projects/myproject

# Dry run to preview
yaqeen init myproject.md --dry-run

# Verbose output
yaqeen init myproject.md --verbose
```

### Advanced Markdown Features

#### Multiple Code Blocks

If your markdown has multiple code blocks, Yaqeen uses the first one that looks like a project structure:

````markdown
# My Project

Some description here.

Code example:
```python
print("Hello")
```

Project structure:
```
myproject/
├── src/
└── README.md
```
````

#### UTF-8 Tree Characters

Yaqeen supports various tree character styles:

```
# Style 1 (ASCII)
myproject/
|-- src/
|   |-- main.cpp
|   `-- utils/
`-- README.md

# Style 2 (Unicode box-drawing)
myproject/
├── src/
│   ├── main.cpp
│   └── utils/
└── README.md

# Style 3 (Simple)
myproject/
  src/
    main.cpp
    utils/
  README.md
```

All styles work interchangeably.

## Understanding Project Creation Process

### Internal Flow

When you run `yaqeen create`, here's what happens:

1. **Template Loading**
   - Yaqeen loads template JSON from `templates/` directory
   - Validates template schema
   - Parses structure definition

2. **Path Resolution**
   - Resolves output directory (absolute or relative)
   - Checks for existing directory conflicts
   - Creates parent directories if needed

3. **Structure Generation**
   - Traverses template structure tree
   - Creates directories depth-first
   - Writes files with template content

4. **Post-Processing**
   - Generates final statistics
   - Writes logs if requested
   - Returns success/error code

### Error Handling

Yaqeen provides descriptive error messages:

**Directory already exists:**
```
[!] Error: Directory 'my-app' already exists
    Use --force to overwrite or choose a different name
```

**Invalid template:**
```
[!] Error: Template 'invalid-template' not found
    Run 'yaqeen list' to see available templates
```

**Permission denied:**
```
[!] Error: Permission denied creating directory '/root/my-app'
    Check your permissions or choose a different output directory
```

## Practical Examples

### Example 1: Full-Stack Application

Create a complete full-stack application with backend and frontend:

```bash
# Create backend
yaqeen create --template laravel --name my-app-api --output ./my-app/backend

# Create frontend
yaqeen create --template react-typescript --name my-app-web --output ./my-app/frontend

# Create shared packages
yaqeen create --template monorepo --name my-app --output ./my-app
```

### Example 2: Clean Architecture Project

```bash
yaqeen create --template clean-architecture --name order-service
cd order-service
```

This creates a structure following clean architecture principles:
```
order-service/
├── domain/           # Entities, value objects, domain services
├── application/      # Use cases, interfaces
├── infrastructure/   # Frameworks, databases, external services
└── presentation/     # Controllers, views, API
```

### Example 3: Microservices

```bash
# Create microservices architecture
yaqeen create --template microservices --name my-platform

cd my-platform

# Each service is a separate module
cd services/auth-service && npm install
cd ../user-service && npm install
cd ../api-gateway && npm install
```

### Example 4: Library/SDK

Creating a reusable library:

```bash
yaqeen create --template library --name my-sdk
cd my-sdk
```

Structure includes:
```
my-sdk/
├── src/           # Source code
├── include/       # Public headers
├── tests/         # Unit tests
├── examples/      # Usage examples
├── docs/          # Documentation
└── CMakeLists.txt # Build configuration
```

## Tips and Best Practices

### 1. Use Dry Run for Exploration

Before creating a project, preview it:

```bash
yaqeen create --template nextjs --name test --dry-run
```

This shows what would be created without actually creating files.

### 2. Combine Templates with Version Control

After creating a project:

```bash
yaqeen create --template django --name myapi
cd myapi
git init
git add .
git commit -m "Initial commit from Yaqeen template"
```

### 3. Customize After Creation

Templates provide a starting point. Customize them to fit your needs:

```bash
yaqeen create --template react-typescript --name my-app
cd my-app
# Add your custom folders
mkdir src/api src/store src/styles
```

### 4. Keep Logs for Debugging

When creating complex projects:

```bash
yaqeen create --template microservices --name platform --verbose --log-file creation.log
```

Review logs if something goes wrong.

### 5. Use Configuration Files

Create a `.yaqeen.json` in your workspace for default settings:

```json
{
  "default_output_dir": "./projects",
  "verbose": true,
  "templates_dir": "./my-templates"
}
```

## Troubleshooting

### Project Creation Fails

**Problem:** "Permission denied" error

**Solution:**
```bash
# Use a directory you have write access to
yaqeen create --template react --name my-app --output ~/projects/my-app
```

### Template Not Found

**Problem:** "Template 'xyz' not found"

**Solution:**
```bash
# List available templates
yaqeen list

# Use exact template name
yaqeen create --template react-typescript --name my-app
```

### Markdown Parsing Fails

**Problem:** "Failed to parse markdown structure"

**Solution:**
- Ensure your markdown has a proper code block with triple backticks
- Use correct tree syntax with proper indentation
- Check for UTF-8 encoding

### Slow Performance

**Problem:** Project creation takes too long

**Possible causes:**
- Very large templates (1000+ files)
- Slow disk I/O
- Network-mounted directories

**Solution:**
```bash
# Use local disk
# Build release version for maximum performance
# Use SSD instead of HDD
```

## Next Steps

Now that you've created your first project:

1. **Learn CLI commands** - See [CLI Reference](../usage/cli-reference.md)
2. **Explore templates** - Browse [Template Overview](../templates/overview.md)
3. **Create custom templates** - Read [Custom Templates Guide](../templates/custom-templates.md)
4. **Advanced features** - Check [Advanced Configuration](../advanced/configuration.md)
5. **Integrate with CI/CD** - See [CI/CD Integration](../advanced/ci-cd.md)

## Summary

You've learned:
- ✅ How to list and inspect templates
- ✅ Creating projects from templates
- ✅ Creating projects from markdown
- ✅ Understanding the creation process
- ✅ Troubleshooting common issues
- ✅ Best practices and tips

Continue exploring Yaqeen's features to maximize your productivity!
