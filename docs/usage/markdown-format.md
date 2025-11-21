# Markdown Format Specification

Yaqeen supports creating project structures directly from markdown files. This document provides a comprehensive specification of the markdown format and parser behavior.

## Overview

The markdown parser (`md4c` library wrapper) extracts project structures from code blocks within markdown files. This enables you to document and generate your project structure simultaneously.

## Basic Syntax

### Minimal Example

````markdown
# My Project

```
myproject/
├── src/
└── README.md
```
````

This creates:
```
myproject/
├── src/
└── README.md
```

### Syntax Rules

1. **Code Block Required**: Structure must be in a fenced code block (triple backticks)
2. **Tree Format**: Use tree-like syntax with box-drawing characters
3. **Directory Indicator**: Directories end with forward slash `/`
4. **File Indicator**: Files have no trailing slash
5. **Nesting**: Indentation or tree characters indicate parent-child relationships

## Tree Character Support

Yaqeen parser supports multiple tree character styles for maximum flexibility.

### Style 1: Unicode Box-Drawing (Recommended)

```
myproject/
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── engine.cpp
│   │   └── engine.hpp
│   └── utils/
└── README.md
```

**Characters used:**
- `├──` - Branch (U+251C, U+2500, U+2500)
- `└──` - Last branch (U+2514, U+2500, U+2500)
- `│` - Vertical line (U+2502)

### Style 2: ASCII Compatible

```
myproject/
|-- src/
|   |-- main.cpp
|   |-- core/
|   |   |-- engine.cpp
|   |   `-- engine.hpp
|   `-- utils/
`-- README.md
```

**Characters used:**
- `|--` - Branch
- `` `-- `` - Last branch
- `|` - Vertical line

### Style 3: Simple Indentation

```
myproject/
  src/
    main.cpp
    core/
      engine.cpp
      engine.hpp
    utils/
  README.md
```

**Indentation:**
- 2 spaces per level (recommended)
- 4 spaces per level (also supported)
- Tabs (supported, converted to 4 spaces)

### Style 4: Mixed

You can mix styles within the same structure:

```
myproject/
├─ src/
   ├─ main.cpp
   └─ core/
      |-- engine.cpp
      `-- engine.hpp
└─ README.md
```

## Parser Behavior

### How the Parser Works

1. **Markdown Parsing**
   - Parse markdown file using md4c library
   - Extract all fenced code blocks
   - Look for code blocks containing tree structures

2. **Structure Detection**
   - Scan code blocks for tree characters or proper indentation
   - Identify root node (first line, typically ends with `/`)
   - Build hierarchical structure

3. **Node Processing**
   - Clean tree characters from lines
   - Extract file/directory names
   - Determine nesting level from indentation
   - Build tree data structure

4. **Validation**
   - Ensure valid naming (no illegal characters)
   - Check path depth limits (default: 256 levels)
   - Validate directory/file indicators

### Parser Algorithm

```cpp
// Simplified pseudocode
Node* parse_markdown(string content) {
    // 1. Extract code blocks
    auto blocks = extract_code_blocks(content);

    // 2. Find structure block
    for (auto& block : blocks) {
        if (is_tree_structure(block)) {
            // 3. Parse tree
            auto root = new Node();
            auto lines = split_lines(block);

            for (size_t i = 0; i < lines.size(); ++i) {
                auto line = lines[i];

                // 4. Calculate depth
                int depth = calculate_depth(line);

                // 5. Extract name
                string name = extract_name(line);
                bool is_dir = ends_with(name, '/');

                // 6. Create node
                auto node = new Node{
                    name: trim(name, '/'),
                    type: is_dir ? DIR : FILE,
                    children: {}
                };

                // 7. Insert into tree
                insert_at_depth(root, node, depth);
            }

            return root;
        }
    }

    return nullptr;
}
```

### Depth Calculation

The parser calculates indentation depth to determine parent-child relationships:

```cpp
int calculate_depth(const string& line) {
    int depth = 0;
    int pos = 0;

    // Count leading spaces
    while (pos < line.size() && line[pos] == ' ') {
        ++pos;
    }

    // Skip tree characters
    if (starts_with(line.substr(pos), "├──") ||
        starts_with(line.substr(pos), "└──") ||
        starts_with(line.substr(pos), "|--") ||
        starts_with(line.substr(pos), "`--")) {
        pos += 3;
    }

    // Calculate depth (assuming 2 spaces = 1 level)
    depth = pos / 2;

    return depth;
}
```

## Advanced Features

### Multiple Code Blocks

If your markdown contains multiple code blocks, Yaqeen uses the **first** block that looks like a tree structure:

````markdown
# My Project

Documentation here.

Usage example:
```python
print("Hello")
```

Project structure:
```
myproject/        ← This block will be used
├── src/
└── README.md
```

Another code block:
```
not/
├── used/
```
````

### Language Specifiers

Code blocks can have language specifiers - Yaqeen ignores them:

````markdown
```text
myproject/
├── src/
└── README.md
```

```tree
myproject/
├── src/
└── README.md
```

```
myproject/
├── src/
└── README.md
```
````

All three are treated identically.

### Empty Directories

Create empty directories by listing them without children:

```
myproject/
├── src/
├── include/
├── lib/
└── build/
```

All four are created as empty directories.

### Deeply Nested Structures

Support for deep nesting (up to 256 levels by default):

```
myproject/
└── very/
    └── deeply/
        └── nested/
            └── structure/
                └── file.txt
```

### Special Characters in Names

File and directory names can contain most characters:

```
myproject/
├── src/
│   ├── hello-world.cpp
│   ├── utils_v2.hpp
│   ├── config.yaml
│   └── data.json
├── .gitignore
├── .github/
│   └── workflows/
├── README.md
└── LICENSE
```

**Allowed:**
- Alphanumeric: `a-z A-Z 0-9`
- Hyphens: `-`
- Underscores: `_`
- Dots: `.`
- Spaces (discouraged)

**Not allowed:**
- Null character: `\0`
- Path separators: `/` (except as directory indicator)
- Control characters

### Comments and Annotations

Yaqeen ignores non-structure lines:

```
Project root
myproject/
├── src/          # Source files
│   ├── main.cpp  # Entry point
│   └── utils/    # Utility functions
└── README.md     # Documentation
```

Everything after structure names is ignored.

## Complex Examples

### Example 1: Full C++ Project

````markdown
# My C++ Application

A high-performance C++ application.

## Structure

```
my-cpp-app/
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── application.cpp
│   │   ├── application.hpp
│   │   ├── window.cpp
│   │   └── window.hpp
│   ├── rendering/
│   │   ├── renderer.cpp
│   │   ├── renderer.hpp
│   │   ├── shader.cpp
│   │   └── shader.hpp
│   └── utils/
│       ├── logger.cpp
│       └── logger.hpp
├── include/
│   └── myapp/
│       ├── core/
│       │   ├── application.hpp
│       │   └── window.hpp
│       └── rendering/
│           ├── renderer.hpp
│           └── shader.hpp
├── tests/
│   ├── test_main.cpp
│   ├── core/
│   │   ├── test_application.cpp
│   │   └── test_window.cpp
│   └── CMakeLists.txt
├── third_party/
│   ├── glfw/
│   ├── glad/
│   └── glm/
├── assets/
│   ├── shaders/
│   │   ├── vertex.glsl
│   │   └── fragment.glsl
│   ├── textures/
│   └── models/
├── docs/
│   ├── README.md
│   ├── API.md
│   └── ARCHITECTURE.md
├── build/
├── CMakeLists.txt
├── .gitignore
├── .clang-format
├── LICENSE
└── README.md
```
````

### Example 2: Monorepo

````markdown
# Platform Monorepo

```
platform/
├── apps/
│   ├── web/
│   │   ├── src/
│   │   ├── public/
│   │   ├── package.json
│   │   └── tsconfig.json
│   ├── mobile/
│   │   ├── src/
│   │   ├── android/
│   │   ├── ios/
│   │   └── package.json
│   └── admin/
│       ├── src/
│       ├── package.json
│       └── tsconfig.json
├── packages/
│   ├── ui-components/
│   │   ├── src/
│   │   ├── package.json
│   │   └── tsconfig.json
│   ├── utils/
│   │   ├── src/
│   │   ├── package.json
│   │   └── tsconfig.json
│   └── api-client/
│       ├── src/
│       ├── package.json
│       └── tsconfig.json
├── services/
│   ├── auth/
│   │   ├── src/
│   │   └── Dockerfile
│   ├── api/
│   │   ├── src/
│   │   └── Dockerfile
│   └── worker/
│       ├── src/
│       └── Dockerfile
├── infrastructure/
│   ├── terraform/
│   ├── kubernetes/
│   └── docker/
├── docs/
├── scripts/
├── .github/
│   └── workflows/
├── package.json
├── pnpm-workspace.yaml
├── turbo.json
└── README.md
```
````

### Example 3: Microservices Architecture

````markdown
# Microservices Platform

```
microservices-platform/
├── services/
│   ├── auth-service/
│   │   ├── src/
│   │   ├── tests/
│   │   ├── Dockerfile
│   │   └── package.json
│   ├── user-service/
│   │   ├── src/
│   │   ├── tests/
│   │   ├── Dockerfile
│   │   └── package.json
│   ├── product-service/
│   │   ├── src/
│   │   ├── tests/
│   │   ├── Dockerfile
│   │   └── package.json
│   └── order-service/
│       ├── src/
│       ├── tests/
│       ├── Dockerfile
│       └── package.json
├── api-gateway/
│   ├── src/
│   ├── config/
│   └── Dockerfile
├── shared/
│   ├── proto/
│   ├── types/
│   └── utils/
├── infrastructure/
│   ├── k8s/
│   │   ├── deployments/
│   │   ├── services/
│   │   └── ingress/
│   └── monitoring/
│       ├── prometheus/
│       └── grafana/
├── docs/
├── scripts/
└── docker-compose.yml
```
````

## Error Handling

### Common Parsing Errors

**Error 1: No code block found**
```
Error: No project structure found in markdown file
Cause: Missing code block or invalid markdown format
Solution: Ensure structure is in a fenced code block (triple backticks)
```

**Error 2: Invalid tree structure**
```
Error: Failed to parse tree structure
Cause: Malformed tree syntax or inconsistent indentation
Solution: Check indentation and tree characters
```

**Error 3: Empty structure**
```
Error: Project structure is empty
Cause: Code block has no content
Solution: Add at least one file or directory
```

**Error 4: Invalid characters**
```
Error: Invalid character in name: 'filename?.txt'
Cause: Illegal characters in file/directory names
Solution: Remove special characters like ?, *, :, etc.
```

### Debugging Parser Issues

Enable verbose mode to see detailed parsing information:

```bash
yaqeen init structure.md --verbose
```

Output includes:
```
[DEBUG] Found 3 code blocks
[DEBUG] Block 1: Python code (skipped)
[DEBUG] Block 2: Tree structure (selected)
[DEBUG] Parsing tree structure...
[DEBUG] Line 1: 'myproject/' - depth 0, type DIR
[DEBUG] Line 2: '├── src/' - depth 1, type DIR
[DEBUG] Line 3: '│   ├── main.cpp' - depth 2, type FILE
[DEBUG] Line 4: '└── README.md' - depth 1, type FILE
[DEBUG] Built tree with 4 nodes
```

## Best Practices

### 1. Use Consistent Indentation

**Good:**
```
myproject/
├── src/
│   ├── main.cpp
│   └── utils.cpp
└── README.md
```

**Bad:**
```
myproject/
├── src/
│    ├── main.cpp    # Inconsistent spacing
│  └── utils.cpp
└── README.md
```

### 2. Use UTF-8 Encoding

Always save markdown files as UTF-8 to ensure proper box-drawing character rendering.

### 3. Document Structure Decisions

Add comments explaining structure choices:

````markdown
# Project Structure

We use clean architecture:

```
myproject/
├── domain/        # Business logic and entities
├── application/   # Use cases and interfaces
├── infrastructure/# External dependencies
└── presentation/  # UI and controllers
```
````

### 4. Keep It Simple

Avoid overly complex structures. Use subdirectories to organize:

**Good:**
```
myproject/
├── src/
│   ├── core/
│   ├── utils/
│   └── api/
└── tests/
```

**Bad:**
```
myproject/
├── src_core_module_primary_impl_v2/
├── src_utils_helper_functions_new/
└── test_unit_integration_e2e_all/
```

### 5. Use Templates for Common Patterns

For frequently used structures, convert them to Yaqeen templates instead of markdown files.

## Performance Considerations

### Parser Performance

- **Small files (< 100 lines)**: < 1ms parsing time
- **Medium files (100-1000 lines)**: 1-10ms parsing time
- **Large files (> 1000 lines)**: 10-50ms parsing time

### Memory Usage

- **Node overhead**: ~64 bytes per node
- **String overhead**: Depends on name length
- **Example**: 100-file project ≈ 10KB memory

### Optimization Tips

1. **Avoid excessive nesting** - Keep depth reasonable (< 10 levels)
2. **Minimize file count** - Large structures (1000+ files) are slower
3. **Use simple tree characters** - ASCII is slightly faster than Unicode
4. **Remove unnecessary whitespace** - Cleaner parsing

## Integration with IDEs

### VS Code

Create a snippet for quick structure generation:

```json
{
  "Yaqeen Project Structure": {
    "prefix": "yaqeen-struct",
    "body": [
      "# ${1:Project Name}",
      "",
      "```",
      "${2:project}/",
      "├── src/",
      "│   └── main.${3:cpp}",
      "├── tests/",
      "└── README.md",
      "```"
    ]
  }
}
```

### Vim

Add abbreviation:

```vim
iab yaqeen # Project<CR><CR>```<CR>project/<CR>├── src/<CR>└── README.md<CR>```
```

## API Access

Use Yaqeen as a library to parse markdown programmatically:

```cpp
#include <yaqeen/core/parser.hpp>

yaqeen::MarkdownParser parser;
auto result = parser.parse_file("structure.md");

if (result.is_ok()) {
    auto root = result.value();
    // Process structure
} else {
    std::cerr << "Error: " << result.error() << std::endl;
}
```

## Conclusion

The markdown format provides a flexible, human-readable way to define project structures. Key takeaways:

- ✅ Use code blocks with tree syntax
- ✅ Support multiple tree character styles
- ✅ Directories end with `/`
- ✅ Parser handles indentation and nesting
- ✅ Use UTF-8 encoding for best results

For more examples, see the [Templates Guide](templates.md) or [API Documentation](../api/library-usage.md).
