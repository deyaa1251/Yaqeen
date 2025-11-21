# Interactive Mode

Yaqeen features a beautiful, Tokyo Night-themed terminal UI for interactive project creation. This mode provides a guided experience with keyboard navigation and real-time preview.

## Overview

Interactive mode is powered by [FTXUI](https://github.com/ArthurSonzogni/FTXUI), a modern C++ terminal UI library. It provides:

- 🎨 **Beautiful UI** - Tokyo Night color scheme with smooth animations
- ⌨️ **Keyboard navigation** - Vim-style and arrow key support
- 🔍 **Real-time search** - Filter templates as you type
- 👁️ **Live preview** - See project structure before creating
- 📊 **Progress indicators** - Visual feedback during creation

## Launching Interactive Mode

### Method 1: Flag

```bash
yaqeen create --interactive
# or
yaqeen create -i
```

### Method 2: No Arguments

Running `yaqeen create` without arguments automatically launches interactive mode:

```bash
yaqeen create
```

### Method 3: Dedicated Command

```bash
yaqeen interactive
```

## UI Components

### Main Screen

```
╔════════════════════════════════════════════════════════════════╗
║                                                                ║
║   ██    ██  █████   ██████  ███████ ███████ ███    ██         ║
║    ██  ██  ██   ██ ██    ██ ██      ██      ████   ██         ║
║     ████   ███████ ██    ██ █████   █████   ██ ██  ██         ║
║      ██    ██   ██ ██  ▄▄██ ██      ██      ██  ██ ██         ║
║      ██    ██   ██  ██████  ███████ ███████ ██   ████         ║
║                                                                ║
║              Project Structure Generator                       ║
║                      Version 1.0.0                             ║
╚════════════════════════════════════════════════════════════════╝

┌─ Template Selection ─────────────────────────────────────────┐
│                                                              │
│ Search: _______________                                      │
│                                                              │
│ Category: [All ▼]                                            │
│                                                              │
│ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓ │
│ ┃ ▸ react-typescript                                    ┃ │
│ ┃   React + TypeScript + Vite                          ┃ │
│ ┃                                                        ┃ │
│ ┃   vue3                                                ┃ │
│ ┃   Vue 3 with Composition API                         ┃ │
│ ┃                                                        ┃ │
│ ┃   nextjs                                              ┃ │
│ ┃   Next.js with App Router                            ┃ │
│ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛ │
│                                                              │
│ [↑↓] Navigate [Enter] Select [Tab] Next Field [Esc] Exit   │
└──────────────────────────────────────────────────────────────┘
```

### Template Details Panel

When a template is selected:

```
┌─ Template Details ───────────────────────────────────────────┐
│                                                              │
│ Name:        react-typescript                                │
│ Category:    web                                             │
│ Version:     1.0.0                                           │
│ Tags:        react, typescript, vite, frontend               │
│                                                              │
│ Description:                                                 │
│ Modern React application with TypeScript and Vite.          │
│ Includes component structure, hooks, utilities, and         │
│ testing setup.                                               │
│                                                              │
│ Features:                                                    │
│  ✓ React 18 with hooks                                      │
│  ✓ TypeScript for type safety                               │
│  ✓ Vite for fast builds                                     │
│  ✓ ESLint + Prettier                                        │
│  ✓ Vitest for testing                                       │
│                                                              │
│ Structure Preview:                                           │
│  my-app/                                                     │
│  ├── src/                                                    │
│  │   ├── App.tsx                                            │
│  │   ├── main.tsx                                           │
│  │   ├── components/                                        │
│  │   ├── hooks/                                             │
│  │   └── utils/                                             │
│  ├── public/                                                 │
│  ├── tests/                                                  │
│  └── package.json                                            │
│                                                              │
│ Estimated: 18 files, 8 directories                          │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### Project Configuration

```
┌─ Project Configuration ──────────────────────────────────────┐
│                                                              │
│ Template:     react-typescript                               │
│                                                              │
│ Project Name: ____________________                           │
│               (Required)                                     │
│                                                              │
│ Output Path:  ____________________                           │
│               (Default: ./project-name)                      │
│                                                              │
│ Options:                                                     │
│  [ ] Dry run (preview only)                                 │
│  [ ] Verbose output                                          │
│  [ ] Force overwrite                                         │
│                                                              │
│ [Tab] Next [Shift+Tab] Previous [Enter] Create [Esc] Back   │
└──────────────────────────────────────────────────────────────┘
```

### Creation Progress

```
┌─ Creating Project ───────────────────────────────────────────┐
│                                                              │
│ Project: my-awesome-app                                      │
│ Template: react-typescript v1.0.0                            │
│                                                              │
│ Progress:                                                    │
│ ████████████████████████░░░░░░░░░ 65% (12/18 files)        │
│                                                              │
│ Current:                                                     │
│  [✓] Creating directories...                                │
│  [✓] Writing configuration files...                         │
│  [*] Generating source files...                             │
│  [ ] Finalizing...                                           │
│                                                              │
│ Recent:                                                      │
│  ✓ Created src/App.tsx                                      │
│  ✓ Created src/main.tsx                                     │
│  ✓ Created src/components/Button.tsx                        │
│  * Creating src/hooks/useCounter.ts                         │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### Completion Screen

```
┌─ Success! ───────────────────────────────────────────────────┐
│                                                              │
│             ✨ Project Created Successfully! ✨              │
│                                                              │
│ Project:      my-awesome-app                                 │
│ Location:     ./my-awesome-app                               │
│ Template:     react-typescript v1.0.0                        │
│                                                              │
│ Statistics:                                                  │
│  📁 Directories:  8                                          │
│  📄 Files:        18                                         │
│  ⏱️  Time:         127ms                                      │
│  💾 Size:         45 KB                                      │
│                                                              │
│ Next Steps:                                                  │
│  1. cd my-awesome-app                                        │
│  2. npm install                                              │
│  3. npm run dev                                              │
│                                                              │
│ [Enter] Create Another [Esc] Exit                           │
└──────────────────────────────────────────────────────────────┘
```

## Keyboard Controls

### Navigation

| Key | Action |
|-----|--------|
| `↑` / `k` | Move up |
| `↓` / `j` | Move down |
| `←` / `h` | Move left / Previous field |
| `→` / `l` | Move right / Next field |
| `PgUp` | Page up |
| `PgDn` | Page down |
| `Home` | First item |
| `End` | Last item |

### Selection

| Key | Action |
|-----|--------|
| `Enter` | Confirm / Select |
| `Space` | Toggle checkbox |
| `Tab` | Next field |
| `Shift+Tab` | Previous field |

### Search

| Key | Action |
|-----|--------|
| `/` | Focus search box |
| `Esc` | Clear search / Exit |
| `Ctrl+C` | Exit immediately |

### Other

| Key | Action |
|-----|--------|
| `?` | Show help |
| `r` | Refresh templates |
| `Ctrl+L` | Redraw screen |

## Features in Detail

### 1. Template Search

**Real-time filtering:**
```
Search: react█

Results:
 ▸ react-typescript
   react-native
```

**Multi-word search:**
```
Search: python fast█

Results:
 ▸ fastapi
   python-project
```

**Tag-based search:**
```
Search: #typescript█

Results:
 ▸ react-typescript
   vue3
   nextjs
   angular
   express
   nestjs
```

### 2. Category Filtering

Use dropdown to filter by category:

```
Category: [Web ▼]

Showing 7 of 35 templates:
 ▸ react-typescript
   vue3
   nextjs
   angular
   svelte
   solidjs
   astro
```

### 3. Template Preview

See full structure before creating:

```
┌─ Structure Preview ──────────────────────────────────────────┐
│ my-app/                                                      │
│ ├── src/                                                     │
│ │   ├── App.tsx                                             │
│ │   ├── App.css                                             │
│ │   ├── main.tsx                                            │
│ │   ├── index.css                                           │
│ │   ├── components/                                         │
│ │   │   └── Button.tsx                                      │
│ │   ├── hooks/                                              │
│ │   │   └── useCounter.ts                                   │
│ │   ├── utils/                                              │
│ │   │   └── helpers.ts                                      │
│ │   └── types/                                              │
│ │       └── index.ts                                        │
│ ├── public/                                                  │
│ │   └── vite.svg                                            │
│ ├── tests/                                                   │
│ │   └── App.test.tsx                                        │
│ ├── .gitignore                                              │
│ ├── package.json                                             │
│ ├── tsconfig.json                                            │
│ ├── vite.config.ts                                          │
│ └── README.md                                                │
│                                                              │
│ [Space] Expand/Collapse [Enter] Select                      │
└──────────────────────────────────────────────────────────────┘
```

### 4. Validation

Real-time input validation:

```
Project Name: my app█
             ^^^
Error: Invalid characters. Use letters, numbers, hyphens, and underscores.

Project Name: my-app█
✓ Valid name
```

```
Output Path: /root/project█
            ^^^^^
Warning: May require elevated permissions

Output Path: ./my-app█
✓ Valid path
```

### 5. Dry Run Mode

Preview what will be created:

```
┌─ Dry Run Results ────────────────────────────────────────────┐
│                                                              │
│ The following would be created:                              │
│                                                              │
│ 📁 ./my-app/                                                 │
│ 📁 ./my-app/src/                                             │
│ 📄 ./my-app/src/App.tsx                (342 bytes)          │
│ 📄 ./my-app/src/main.tsx               (187 bytes)          │
│ 📁 ./my-app/src/components/                                  │
│ 📄 ./my-app/src/components/Button.tsx  (256 bytes)          │
│ ...                                                          │
│                                                              │
│ Total: 18 files, 8 directories, 45 KB                       │
│                                                              │
│ [Enter] Create For Real [Esc] Cancel                        │
└──────────────────────────────────────────────────────────────┘
```

### 6. Error Handling

Clear error messages:

```
┌─ Error ──────────────────────────────────────────────────────┐
│                                                              │
│ ❌ Failed to create project                                  │
│                                                              │
│ Error: Directory 'my-app' already exists                     │
│                                                              │
│ Solutions:                                                   │
│  1. Choose a different project name                          │
│  2. Delete the existing directory                            │
│  3. Enable "Force overwrite" option                          │
│                                                              │
│ [Enter] Try Again [Esc] Cancel                              │
└──────────────────────────────────────────────────────────────┘
```

## Color Scheme

Yaqeen uses the Tokyo Night color palette:

```cpp
// Tokyo Night Colors
namespace theme {
    constexpr Color background = Color::RGB(0x1a, 0x1b, 0x26);   // #1a1b26
    constexpr Color foreground = Color::RGB(0xc0, 0xca, 0xf5);   // #c0caf5
    constexpr Color selection  = Color::RGB(0x28, 0x2c, 0x34);   // #282c34
    constexpr Color comment    = Color::RGB(0x56, 0x5f, 0x89);   // #565f89
    constexpr Color cyan       = Color::RGB(0x7d, 0xca, 0xfe);   // #7dcafe
    constexpr Color blue       = Color::RGB(0x7a, 0xa2, 0xf7);   // #7aa2f7
    constexpr Color magenta    = Color::RGB(0xbb, 0x9a, 0xf7);   // #bb9af7
    constexpr Color red        = Color::RGB(0xf7, 0x76, 0x8e);   // #f7768e
    constexpr Color orange     = Color::RGB(0xff, 0x9e, 0x64);   // #ff9e64
    constexpr Color yellow     = Color::RGB(0xe0, 0xaf, 0x68);   // #e0af68
    constexpr Color green      = Color::RGB(0x9e, 0xce, 0x6a);   // #9ece6a
    constexpr Color teal       = Color::RGB(0x1a, 0xbc, 0x9c);   // #1abc9c
}
```

**Element colors:**
- **Borders:** Blue (`#7aa2f7`)
- **Selections:** Cyan (`#7dcafe`)
- **Success:** Green (`#9ece6a`)
- **Warnings:** Yellow (`#e0af68`)
- **Errors:** Red (`#f7768e`)
- **Info:** Magenta (`#bb9af7`)

## Animations

### Spinner

During operations:
```
[⠋] Loading templates...
[⠙] Loading templates...
[⠹] Loading templates...
[⠸] Loading templates...
[⠼] Loading templates...
[⠴] Loading templates...
[⠦] Loading templates...
[⠧] Loading templates...
[⠇] Loading templates...
[⠏] Loading templates...
```

### Progress Bar

```
████████████████████████░░░░░░░░░ 65%
```

Smooth animation from 0% to 100%.

### Fade In/Out

Screen transitions use smooth fade effects.

## Configuration

### Default Options

Set defaults in `~/.yaqeen/config.json`:

```json
{
  "interactive": {
    "theme": "tokyo-night",
    "animations": true,
    "vim_keys": true,
    "template_preview": true,
    "confirm_on_exit": true
  }
}
```

### Disable Interactive Mode

Force CLI mode:

```bash
yaqeen create --template react --name app --no-interactive
```

### Custom Theme

Create custom theme (advanced):

```json
{
  "theme": {
    "name": "custom",
    "colors": {
      "background": "#1e1e1e",
      "foreground": "#d4d4d4",
      "primary": "#007acc",
      "success": "#4ec9b0",
      "error": "#f48771"
    }
  }
}
```

## Performance

### Rendering Performance

- **60 FPS** - Smooth animations
- **< 16ms** - Frame render time
- **< 1MB** - Memory overhead

### Template Loading

- **< 10ms** - Load all templates
- **< 1ms** - Filter/search
- **< 5ms** - Build preview

### Responsiveness

UI remains responsive even with:
- 1000+ templates
- Large project structures
- Slow terminals

## Accessibility

### Screen Readers

Yaqeen provides text descriptions for screen readers:

```
Template: react-typescript
Description: React with TypeScript and Vite
Status: Selected
Press Enter to configure
```

### High Contrast Mode

Enable high contrast:

```bash
yaqeen create --interactive --high-contrast
```

### Large Text

Increase text size:

```bash
yaqeen create --interactive --large-text
```

## Troubleshooting

### UI Not Rendering

**Problem:** Box-drawing characters not displaying

**Solution:**
```bash
# Use UTF-8 terminal
export LANG=en_US.UTF-8

# Or use ASCII-only mode
yaqeen create --interactive --ascii-only
```

### Colors Not Working

**Problem:** No colors in terminal

**Solution:**
```bash
# Check terminal supports colors
echo $TERM

# Should be xterm-256color or similar
# If not, set it:
export TERM=xterm-256color

# Or force color:
yaqeen create --interactive --force-color
```

### Slow Performance

**Problem:** Laggy UI

**Solution:**
```bash
# Disable animations
yaqeen create --interactive --no-animations

# Or use CLI mode
yaqeen create --template react --name app
```

### Vim Keys Not Working

**Problem:** hjkl navigation not working

**Solution:**
```bash
# Enable Vim keys
yaqeen config set interactive.vim_keys true

# Or use arrow keys
```

## Advanced Usage

### Scripting Interactive Mode

Pipe inputs to interactive mode:

```bash
echo -e "react-typescript\nmy-app\n./output\n" | yaqeen create --interactive
```

### Batch Mode

Create multiple projects:

```bash
yaqeen create --interactive --batch < projects.txt
```

Where `projects.txt`:
```
react-typescript my-app-1
vue3 my-app-2
nextjs my-app-3
```

## Comparison: Interactive vs CLI

| Feature | Interactive | CLI |
|---------|-------------|-----|
| **Ease of use** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ |
| **Speed** | Medium | Fast |
| **Automation** | Limited | Full |
| **Discovery** | Excellent | Manual |
| **Preview** | Built-in | Manual |
| **Learning curve** | Low | Medium |

**Use interactive when:**
- ✅ Exploring templates
- ✅ Learning Yaqeen
- ✅ Unsure of template names
- ✅ Want visual feedback

**Use CLI when:**
- ✅ Automating tasks
- ✅ In scripts
- ✅ SSH/remote sessions
- ✅ CI/CD pipelines

## Examples

### Example 1: Quick Project Creation

```bash
yaqeen create -i

# Select template: react-typescript
# Enter name: my-app
# Press Enter to create
```

### Example 2: Preview Before Creating

```bash
yaqeen create -i

# Select template
# Enable "Dry run" checkbox
# Review preview
# Uncheck "Dry run"
# Create project
```

### Example 3: Custom Output Directory

```bash
yaqeen create -i

# Select template: django
# Enter name: blog-api
# Enter output: ~/projects/blog/backend
# Create
```

## Summary

Interactive mode provides:
- ✅ Beautiful Tokyo Night UI
- ✅ Keyboard-driven navigation
- ✅ Real-time template search
- ✅ Live structure preview
- ✅ Input validation
- ✅ Progress feedback
- ✅ Error handling

Perfect for:
- 🎯 First-time users
- 🔍 Template exploration
- 👁️ Project preview
- ✨ Better user experience

Next steps:
- Practice with `yaqeen create -i`
- Explore [Templates Guide](templates.md)
- Learn [CLI commands](cli-reference.md)
- Check [Configuration](../advanced/configuration.md)
