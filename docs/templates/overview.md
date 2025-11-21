# Templates Overview

Yaqeen comes with 30+ professionally crafted templates covering a wide range of frameworks, languages, and architectural patterns.

## Template Categories

### Web Frontend (7 templates)

Modern frontend frameworks and tools for building web applications.

| Template | Description | Tags |
|----------|-------------|------|
| `react-typescript` | React with TypeScript, Vite, and modern tooling | react, typescript, vite |
| `vue3` | Vue 3 with Composition API and TypeScript | vue, typescript, composition-api |
| `nextjs` | Next.js with App Router and TypeScript | nextjs, react, ssr |
| `angular` | Angular with standalone components | angular, typescript |
| `svelte` | Svelte with SvelteKit | svelte, sveltekit |
| `solidjs` | Solid.js with TypeScript | solidjs, typescript |
| `astro` | Astro static site generator | astro, static-site |

### Backend (10 templates)

Server-side frameworks for building APIs and web applications.

| Template | Description | Language |
|----------|-------------|----------|
| `laravel` | Laravel PHP framework with MVC | PHP |
| `django` | Django Python web framework | Python |
| `fastapi` | FastAPI modern Python API framework | Python |
| `express` | Express.js with TypeScript | Node.js |
| `nestjs` | NestJS progressive Node.js framework | Node.js |
| `spring-boot` | Spring Boot Java framework | Java |
| `aspnet-core` | ASP.NET Core web application | C# |
| `rails` | Ruby on Rails web framework | Ruby |
| `go-gin` | Go with Gin web framework | Go |
| `rust-actix` | Rust with Actix-web framework | Rust |

### Mobile (2 templates)

Cross-platform mobile development frameworks.

| Template | Description | Platform |
|----------|-------------|----------|
| `react-native` | React Native with TypeScript | iOS, Android |
| `flutter` | Flutter with Dart | iOS, Android, Web |

### Architecture Patterns (8 templates)

Proven architectural patterns for scalable applications.

| Template | Description | Use Case |
|----------|-------------|----------|
| `clean-architecture` | Clean Architecture with clear separation | Enterprise apps |
| `hexagonal` | Hexagonal (Ports & Adapters) | Testable systems |
| `ddd` | Domain-Driven Design | Complex domains |
| `mvc` | Model-View-Controller | Web applications |
| `mvvm` | Model-View-ViewModel | UI applications |
| `service-repository` | Service-Repository pattern | Data-driven apps |
| `microservices` | Microservices architecture | Distributed systems |
| `event-driven` | Event-Driven Architecture | Reactive systems |

### Languages (5 templates)

Language-specific project structures.

| Template | Description | Language |
|----------|-------------|----------|
| `cpp-project` | Modern C++ with CMake | C++ |
| `rust-project` | Rust with Cargo | Rust |
| `python-project` | Python package | Python |
| `go-project` | Go with standard layout | Go |
| `java-maven` | Java with Maven | Java |

### Other (3 templates)

Specialized project types.

| Template | Description | Use Case |
|----------|-------------|----------|
| `monorepo` | Monorepo with multiple packages | Large projects |
| `library` | Generic library/SDK structure | Libraries |
| `documentation` | Documentation site | Docs |

## Template Structure

Each template includes:

- **Standard directory layout** - Industry-standard organization
- **Configuration files** - Framework-specific config files
- **README template** - Basic project documentation
- **Gitignore** - Appropriate ignore patterns
- **Package files** - Dependencies and build configuration

## Choosing a Template

### By Use Case

**Building a web app?**
- SPA: `react-typescript`, `vue3`, `svelte`
- SSR: `nextjs`, `angular`
- Static site: `astro`

**Building an API?**
- Fast & modern: `fastapi`, `go-gin`, `rust-actix`
- Full-featured: `laravel`, `django`, `rails`
- Enterprise: `spring-boot`, `aspnet-core`

**Mobile app?**
- Cross-platform: `react-native`, `flutter`

**Library/SDK?**
- Language-specific: `cpp-project`, `rust-project`, `python-project`
- Generic: `library`

### By Architecture

**Need specific architecture?**
- Clean code: `clean-architecture`
- Testability: `hexagonal`
- Complex domain: `ddd`
- Traditional: `mvc`, `mvvm`
- Modern: `microservices`, `event-driven`

## Template Details

Each template provides:

1. **Name** - Unique identifier
2. **Description** - Brief explanation
3. **Category** - Grouping
4. **Tags** - Searchable keywords
5. **Structure** - Directory and file layout
6. **Version** - Template version

View details:
```bash
yaqeen show <template-name>
```

## Using Templates

### Basic Usage

```bash
yaqeen create --template <template-name> --name <project-name>
```

### With Options

```bash
# Custom output directory
yaqeen create -t react-typescript -n myapp -o ./projects/myapp

# Preview before creating
yaqeen create -t laravel -n blog --dry-run

# Verbose output
yaqeen create -t django -n api --verbose
```

## Template Customization

Templates can be customized by:

1. **Modifying templates** - Edit JSON files in `templates/` directory
2. **Creating new templates** - Add new JSON files
3. **Override templates** - Place custom templates in `~/.config/yaqeen/templates/`

See [Custom Templates](custom-templates.md) for details.

## Template Contributions

Want to contribute a template? See our [Contributing Guide](../development/contributing.md).

### Template Requirements

- Follow JSON schema
- Include README
- Provide description and tags
- Test thoroughly
- Document any special requirements

## Next Steps

- [View web templates in detail](web-frontend.md)
- [View backend templates](backend.md)
- [Create custom templates](custom-templates.md)
