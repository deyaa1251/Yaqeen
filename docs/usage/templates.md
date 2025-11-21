# Using Templates

Templates are pre-configured project structures that follow industry best practices for specific frameworks, languages, and architectural patterns. This guide covers everything about working with templates in Yaqeen.

## Template Basics

### What is a Template?

A template is a JSON file that defines:
- **Metadata** - Name, description, version, category, tags
- **Structure** - Directory hierarchy and file organization
- **Content** - Optional file contents and boilerplate code

### Template Location

Templates are stored in:
```
/usr/local/share/yaqeen/templates/     # Linux/macOS system installation
C:\Program Files\yaqeen\templates\     # Windows system installation
~/.yaqeen/templates/                   # User templates
./templates/                           # Local project templates
```

### Template Categories

Yaqeen organizes templates into categories:

| Category | Description | Count |
|----------|-------------|-------|
| `web` | Frontend web frameworks | 7 |
| `backend` | Backend frameworks and APIs | 10 |
| `mobile` | Mobile application frameworks | 2 |
| `patterns` | Architectural patterns | 8 |
| `languages` | Language-specific projects | 5 |
| `other` | Miscellaneous templates | 3 |

## Discovering Templates

### List All Templates

```bash
yaqeen list
```

Output:
```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  AVAILABLE TEMPLATES (35)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

WEB FRONTEND (7)
  react-typescript     React + TypeScript + Vite
  vue3                 Vue 3 with Composition API
  nextjs               Next.js with App Router
  angular              Angular with standalone components
  svelte               Svelte with SvelteKit
  solidjs              Solid.js with TypeScript
  astro                Astro static site generator

BACKEND (10)
  laravel              Laravel PHP framework
  django               Django Python framework
  fastapi              FastAPI Python framework
  express              Express.js with TypeScript
  nestjs               NestJS framework
  spring-boot          Spring Boot Java
  aspnet-core          ASP.NET Core
  rails                Ruby on Rails
  go-gin               Go with Gin framework
  rust-actix           Rust with Actix-web

... (more categories)
```

### Filter by Category

```bash
yaqeen list --category web
yaqeen list --category backend
yaqeen list --category patterns
```

### Search Templates

```bash
# Search by name or tag
yaqeen list --search react
yaqeen list --search typescript
yaqeen list --search api
```

## Inspecting Templates

### Show Template Details

```bash
yaqeen show react-typescript
```

Output:
```
╔═══════════════════════════════════════════════════════╗
║ Template: react-typescript                            ║
║ Description: React + TypeScript + Vite                ║
║ Version: 1.0.0                                        ║
║ Category: web                                         ║
║ Tags: react, typescript, vite, frontend, spa          ║
╚═══════════════════════════════════════════════════════╝

Project Structure:
my-app/
├── src/
│   ├── App.tsx
│   ├── App.css
│   ├── main.tsx
│   ├── index.css
│   ├── components/
│   │   └── Button.tsx
│   ├── hooks/
│   │   └── useCounter.ts
│   ├── utils/
│   │   └── helpers.ts
│   └── types/
│       └── index.ts
├── public/
│   └── vite.svg
├── tests/
│   └── App.test.tsx
├── .gitignore
├── package.json
├── tsconfig.json
├── tsconfig.node.json
├── vite.config.ts
├── README.md
└── LICENSE

Estimated files: 18
Estimated directories: 8
```

### View Template JSON

```bash
# View raw template definition
cat /usr/local/share/yaqeen/templates/web/react-typescript.json
```

## Creating Projects from Templates

### Basic Usage

```bash
yaqeen create --template <template-name> --name <project-name>
```

Example:
```bash
yaqeen create --template react-typescript --name my-app
```

### All Options

```bash
yaqeen create \
  --template react-typescript \
  --name my-app \
  --output ./projects/my-app \
  --dry-run \
  --verbose \
  --log-file creation.log \
  --force
```

**Options explained:**
- `--template, -t` - Template name (required)
- `--name, -n` - Project name (required)
- `--output, -o` - Output directory (default: `./<project-name>`)
- `--dry-run` - Preview without creating files
- `--verbose, -v` - Detailed logging
- `--log-file` - Save logs to file
- `--force, -f` - Overwrite existing directory

### Interactive Mode

Launch interactive template selector:

```bash
yaqeen create --interactive
# or
yaqeen create -i
```

See [Interactive Mode](interactive-mode.md) for details.

## Template Categories in Detail

### Web Frontend Templates

#### react-typescript
Modern React with TypeScript, Vite, and best practices.

**Features:**
- ⚛️ React 18 with hooks
- 📘 TypeScript for type safety
- ⚡ Vite for fast builds
- 🎨 CSS modules support
- 🧪 Vitest for testing
- 🔧 ESLint + Prettier

**Use case:** SPAs, dashboards, web applications

#### vue3
Vue 3 with Composition API and TypeScript.

**Features:**
- 🖖 Vue 3 with Composition API
- 📘 TypeScript support
- ⚡ Vite build tool
- 🎨 Scoped styles
- 🧪 Vitest for testing

**Use case:** Interactive web applications

#### nextjs
Next.js 14 with App Router and Server Components.

**Features:**
- ⚛️ React Server Components
- 🚀 App Router
- 📘 TypeScript
- 🎨 Tailwind CSS
- 🌐 API routes
- 📦 Optimized bundling

**Use case:** Full-stack web applications, SSR, SSG

#### angular
Angular with standalone components (latest version).

**Features:**
- 🅰️ Angular 17+
- 📘 TypeScript
- 🔧 Standalone components
- 🎨 Angular Material ready
- 🧪 Jasmine + Karma testing

**Use case:** Enterprise applications

#### svelte
Svelte with SvelteKit for full-stack apps.

**Features:**
- 🔥 Svelte compiler magic
- ⚡ SvelteKit framework
- 📘 TypeScript
- 🎨 Scoped styles
- 🧪 Vitest testing

**Use case:** Performant web apps with minimal JS

#### solidjs
Solid.js with fine-grained reactivity.

**Features:**
- ⚡ Fine-grained reactivity
- 📘 TypeScript
- 🎨 Solid-styled-components
- 🧪 Vitest testing

**Use case:** High-performance reactive UIs

#### astro
Astro for content-focused websites.

**Features:**
- 🚀 Partial hydration
- 📦 Component framework agnostic
- 📘 TypeScript
- 🎨 Built-in styling
- 📝 MDX support

**Use case:** Blogs, documentation sites, marketing pages

### Backend Templates

#### laravel
Laravel PHP framework for web applications.

**Structure:**
```
my-api/
├── app/
│   ├── Http/
│   ├── Models/
│   ├── Services/
│   └── Providers/
├── database/
│   ├── migrations/
│   └── seeders/
├── routes/
├── tests/
└── composer.json
```

**Use case:** PHP web applications and APIs

#### django
Django Python web framework.

**Structure:**
```
my-project/
├── myproject/
│   ├── settings/
│   ├── urls.py
│   └── wsgi.py
├── apps/
│   └── core/
├── templates/
├── static/
├── requirements.txt
└── manage.py
```

**Use case:** Python web applications with ORM

#### fastapi
FastAPI modern Python API framework.

**Structure:**
```
my-api/
├── app/
│   ├── api/
│   │   └── v1/
│   ├── core/
│   ├── models/
│   ├── schemas/
│   └── main.py
├── tests/
├── requirements.txt
└── pyproject.toml
```

**Use case:** High-performance Python APIs

#### express
Express.js with TypeScript.

**Structure:**
```
my-api/
├── src/
│   ├── controllers/
│   ├── routes/
│   ├── middleware/
│   ├── models/
│   ├── services/
│   └── index.ts
├── tests/
├── package.json
└── tsconfig.json
```

**Use case:** Node.js REST APIs

#### nestjs
NestJS enterprise framework for Node.js.

**Structure:**
```
my-api/
├── src/
│   ├── modules/
│   │   ├── users/
│   │   └── auth/
│   ├── common/
│   ├── config/
│   └── main.ts
├── test/
└── package.json
```

**Use case:** Enterprise-grade Node.js APIs

#### spring-boot
Spring Boot Java framework.

**Structure:**
```
my-api/
├── src/
│   ├── main/
│   │   ├── java/
│   │   │   └── com/myapp/
│   │   │       ├── controller/
│   │   │       ├── service/
│   │   │       ├── repository/
│   │   │       └── model/
│   │   └── resources/
│   └── test/
├── pom.xml
└── application.properties
```

**Use case:** Java enterprise applications

#### go-gin
Go with Gin web framework.

**Structure:**
```
my-api/
├── cmd/
│   └── api/
│       └── main.go
├── internal/
│   ├── handler/
│   ├── service/
│   ├── repository/
│   └── model/
├── pkg/
├── go.mod
└── Makefile
```

**Use case:** High-performance Go APIs

#### rust-actix
Rust with Actix-web framework.

**Structure:**
```
my-api/
├── src/
│   ├── handlers/
│   ├── models/
│   ├── services/
│   └── main.rs
├── tests/
├── Cargo.toml
└── Cargo.lock
```

**Use case:** Ultra-fast, safe Rust APIs

### Mobile Templates

#### react-native
React Native with TypeScript.

**Structure:**
```
my-app/
├── src/
│   ├── screens/
│   ├── components/
│   ├── navigation/
│   └── App.tsx
├── android/
├── ios/
├── package.json
└── tsconfig.json
```

**Use case:** Cross-platform mobile apps

#### flutter
Flutter with Dart.

**Structure:**
```
my-app/
├── lib/
│   ├── screens/
│   ├── widgets/
│   ├── models/
│   └── main.dart
├── test/
├── android/
├── ios/
└── pubspec.yaml
```

**Use case:** Cross-platform mobile apps

### Architecture Pattern Templates

#### clean-architecture
Clean Architecture / Hexagonal Architecture pattern.

**Structure:**
```
my-app/
├── domain/              # Business logic
│   ├── entities/
│   ├── repositories/    # Interfaces
│   └── usecases/
├── application/         # Use cases
│   ├── services/
│   └── dto/
├── infrastructure/      # External dependencies
│   ├── database/
│   ├── api/
│   └── cache/
└── presentation/        # UI/Controllers
    ├── controllers/
    └── views/
```

**Use case:** Maintainable, testable applications

#### ddd
Domain-Driven Design structure.

**Structure:**
```
my-app/
├── domain/
│   ├── aggregates/
│   ├── entities/
│   ├── value-objects/
│   └── domain-services/
├── application/
│   ├── commands/
│   ├── queries/
│   └── handlers/
├── infrastructure/
└── api/
```

**Use case:** Complex domain logic

#### microservices
Microservices architecture.

**Structure:**
```
platform/
├── services/
│   ├── auth/
│   ├── users/
│   ├── products/
│   └── orders/
├── api-gateway/
├── shared/
└── infrastructure/
```

**Use case:** Scalable distributed systems

### Language-Specific Templates

#### cpp-project
Modern C++ project with CMake.

**Structure:**
```
my-project/
├── src/
├── include/
│   └── myproject/
├── tests/
├── third_party/
├── CMakeLists.txt
└── README.md
```

**Use case:** C++ applications and libraries

#### rust-project
Rust project with Cargo.

**Structure:**
```
my-project/
├── src/
│   ├── lib.rs
│   └── main.rs
├── tests/
├── examples/
├── benches/
├── Cargo.toml
└── README.md
```

**Use case:** Rust applications

#### python-project
Python package with modern tooling.

**Structure:**
```
my-package/
├── src/
│   └── mypackage/
├── tests/
├── docs/
├── pyproject.toml
├── setup.py
└── README.md
```

**Use case:** Python packages/applications

## Custom Template Directory

Use templates from a custom location:

```bash
# One-time use
yaqeen create --template my-template --name app --templates-dir ./my-templates

# Set default in config
yaqeen config set templates-dir ~/my-templates
```

## Template Versioning

Templates have semantic versioning (`major.minor.patch`):

```bash
# Use specific version
yaqeen create --template react-typescript@1.2.0 --name app

# Use latest (default)
yaqeen create --template react-typescript --name app
```

## Best Practices

### 1. Choose the Right Template

Match template to your project needs:
- **Small project** → Simple template (e.g., `express`)
- **Enterprise** → Robust template (e.g., `nestjs`, `spring-boot`)
- **Performance-critical** → Optimized template (e.g., `rust-actix`, `go-gin`)

### 2. Customize After Creation

Templates are starting points. Customize them:

```bash
yaqeen create --template react-typescript --name my-app
cd my-app

# Add your custom directories
mkdir src/features src/services src/store

# Install additional dependencies
npm install axios zustand
```

### 3. Understand Template Structure

Before using a template, understand its organization:

```bash
# Preview structure
yaqeen show react-typescript

# Dry run to see files
yaqeen create --template react-typescript --name test --dry-run
```

### 4. Keep Templates Updated

Update Yaqeen to get latest templates:

```bash
# Check for updates
yaqeen update --check

# Update Yaqeen
yaqeen update
```

### 5. Create Custom Templates

For frequently-used structures, create custom templates:

```bash
# Create template from existing project
yaqeen template create --from ./my-project --name my-template

# Or manually create JSON template
cat > my-template.json << 'EOF'
{
  "name": "my-template",
  "description": "My custom template",
  "version": "1.0.0",
  "category": "other",
  "structure": {
    "src/": {},
    "README.md": ""
  }
}
EOF
```

## Template Comparison

### React vs Vue vs Svelte

| Feature | React | Vue | Svelte |
|---------|-------|-----|--------|
| **Learning curve** | Medium | Easy | Easy |
| **Performance** | Good | Good | Excellent |
| **Ecosystem** | Huge | Large | Growing |
| **Bundle size** | Medium | Small | Smallest |
| **Use case** | Complex UIs | Balanced | Performance-critical |

### Express vs NestJS vs Fastify

| Feature | Express | NestJS | Fastify |
|---------|---------|--------|---------|
| **Learning curve** | Easy | Medium | Easy |
| **Performance** | Good | Good | Excellent |
| **Structure** | Minimal | Opinionated | Flexible |
| **TypeScript** | Optional | First-class | First-class |
| **Use case** | Small APIs | Enterprise | High-performance |

### Django vs FastAPI vs Flask

| Feature | Django | FastAPI | Flask |
|---------|--------|---------|-------|
| **Learning curve** | Medium | Easy | Easy |
| **Performance** | Good | Excellent | Good |
| **Features** | Batteries-included | API-focused | Minimal |
| **Async** | Limited | Native | With extensions |
| **Use case** | Full-stack | Modern APIs | Microservices |

## Troubleshooting

### Template Not Found

```bash
Error: Template 'react' not found
```

**Solution:**
```bash
# List available templates
yaqeen list

# Use exact template name
yaqeen create --template react-typescript --name app
```

### Template Load Error

```bash
Error: Failed to load template: Invalid JSON
```

**Solution:**
Check template JSON syntax:
```bash
# Validate template JSON
cat ~/.yaqeen/templates/my-template.json | jq .
```

### Outdated Template

```bash
Warning: Template 'react-typescript' (v1.0.0) is outdated. Latest: v1.5.0
```

**Solution:**
```bash
# Update Yaqeen
yaqeen update
```

## Summary

Key points:
- ✅ 35+ pre-built templates across 6 categories
- ✅ Use `yaqeen list` to discover templates
- ✅ Use `yaqeen show` to inspect templates
- ✅ Choose templates matching your project needs
- ✅ Customize templates after creation
- ✅ Create custom templates for reusable structures

Next steps:
- Learn about [Custom Templates](../templates/custom-templates.md)
- Explore [Interactive Mode](interactive-mode.md)
- See [Template Development](../development/templates.md)
