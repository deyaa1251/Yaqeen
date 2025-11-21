# Quick Start

Get up and running with Yaqeen in just a few minutes!

## Your First Project

The fastest way to get started is to create a project from one of our pre-built templates.

### 1. List Available Templates

First, let's see what templates are available:

```bash
yaqeen list
```

You'll see templates grouped by category:

```
web
  react-typescript        - React application with TypeScript
  vue3                    - Vue 3 application with Composition API
  nextjs                  - Next.js with App Router
  ...

backend
  laravel                 - Laravel PHP framework
  django                  - Django Python framework
  fastapi                 - FastAPI modern Python framework
  ...
```

### 2. Create a Project

Let's create a React + TypeScript project:

```bash
yaqeen create --template react-typescript --name my-awesome-app
```

You'll see beautiful output showing the progress:

```
  ██    ██  █████   ██████  ███████ ███████ ███    ██
   ██  ██  ██   ██ ██    ██ ██      ██      ████   ██
    ████   ███████ ██    ██ █████   █████   ██ ██  ██
     ██    ██   ██ ██  ▄▄██ ██      ██      ██  ██ ██
     ██    ██   ██  ██████  ███████ ███████ ██   ████

          Project Structure Generator

[*] Creating project: my-awesome-app
[*] Template: react-typescript

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

[+] Project created successfully!

  Directories: 15
  Files: 42
  Time: 125ms

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

### 3. Navigate to Your Project

```bash
cd my-awesome-app
```

### 4. Start Building!

Your project is ready. Follow the framework-specific instructions:

For React:
```bash
npm install
npm run dev
```

## Create from Markdown

You can also define your project structure in a markdown file.

### 1. Create a Markdown File

Create `project.md`:

````markdown
# My Custom Project

```
my-project/
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── engine.cpp
│   │   └── engine.hpp
│   └── utils/
│       └── helpers.cpp
├── include/
│   └── myproject/
├── tests/
│   └── test_main.cpp
├── CMakeLists.txt
└── README.md
```
````

### 2. Generate the Project

```bash
yaqeen init project.md
```

Your project structure will be created in the current directory.

## Common Options

### Specify Output Directory

```bash
yaqeen create --template laravel --name blog --output ./projects/my-blog
```

### Dry Run (Preview)

See what will be created without actually creating files:

```bash
yaqeen create --template django --name api --dry-run
```

### Verbose Output

Get detailed information about each file being created:

```bash
yaqeen create --template nextjs --name website --verbose
```

## View Template Details

Before creating a project, you can view details about a template:

```bash
yaqeen show react-typescript
```

Output:
```
╔═══════════════════════════════════════════════╗
║ Template: react-typescript                    ║
║ Description: React with TypeScript            ║
║ Category: web                                 ║
║ Version: 1.0.0                                ║
╚═══════════════════════════════════════════════╝
```

## Next Steps

- Learn about [CLI commands](../usage/cli-reference.md)
- Explore [all available templates](../templates/overview.md)
- Create [custom templates](../templates/custom-templates.md)
- Set up [configuration](../advanced/configuration.md)

## Need Help?

- Check the [FAQ](../faq.md)
- Read the [full documentation](../README.md)
- Report issues on [GitHub](https://github.com/yourusername/yaqeen/issues)
