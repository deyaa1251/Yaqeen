# Architecture

This document provides an in-depth look at Yaqeen's internal architecture, design decisions, and implementation details.

## System Overview

Yaqeen is a high-performance CLI tool written in modern C++17 that generates project structures from templates or markdown files. The architecture emphasizes:

- **Performance** - Optimized for speed with minimal memory overhead
- **Modularity** - Clean separation of concerns
- **Extensibility** - Easy to add new features and templates
- **Reliability** - Comprehensive error handling and validation

## Architecture Diagram

```
┌────────────────────────────────────────────────────────────────┐
│                         CLI Layer                               │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────────┐ │
│  │ CLI11        │  │ FTXUI        │  │ Command Handlers    │ │
│  │ (Arg Parser) │  │ (Terminal UI)│  │ (init, create, etc) │ │
│  └──────────────┘  └──────────────┘  └──────────────────────┘ │
└────────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────────────┐
│                         Core Layer                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────────┐ │
│  │ Parser       │  │ Template     │  │ Generator            │ │
│  │ (Markdown)   │  │ Manager      │  │ (File Creation)      │ │
│  └──────────────┘  └──────────────┘  └──────────────────────┘ │
└────────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────────────┐
│                       Utility Layer                             │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────────┐ │
│  │ Logger       │  │ Error        │  │ Validators           │ │
│  │              │  │ Handler      │  │                      │ │
│  └──────────────┘  └──────────────┘  └──────────────────────┘ │
└────────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────────────┐
│                      External Dependencies                      │
│  │ md4c │ nlohmann/json │ CLI11 │ FTXUI │ Catch2 (tests) │  │
└────────────────────────────────────────────────────────────────┘
```

## Component Details

### 1. CLI Layer

#### Command Line Parser (CLI11)

**Location:** `src/main.cpp`

**Purpose:** Parse command-line arguments and dispatch to appropriate handlers

**Key Features:**
- Subcommand support (`init`, `create`, `list`, `show`)
- Flag validation
- Help generation
- Default values

**Example:**
```cpp
#include <CLI/CLI.hpp>

CLI::App app{"Yaqeen - Project Structure Generator"};

// Global options
bool verbose = false;
bool dry_run = false;
app.add_flag("-v,--verbose", verbose, "Verbose output");
app.add_flag("--dry-run", dry_run, "Preview without creating files");

// Subcommand: create
auto* create_cmd = app.add_subcommand("create", "Create project from template");
std::string template_name;
std::string project_name;
create_cmd->add_option("--template,-t", template_name, "Template name")->required();
create_cmd->add_option("--name,-n", project_name, "Project name")->required();

CLI11_PARSE(app, argc, argv);
```

#### Terminal UI (FTXUI)

**Location:** `src/ui/`

**Purpose:** Interactive terminal user interface

**Components:**
- **`animations.cpp`** - Spinner, progress bar, fade effects
- **`progress.cpp`** - Progress tracking and display
- **`theme.cpp`** - Tokyo Night color scheme

**Example:**
```cpp
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

// Create interactive template selector
auto selector = Menu({
    "react-typescript",
    "vue3",
    "nextjs"
}, &selected_index);

// Render with Tokyo Night colors
auto document = vbox({
    text("Select Template") | bold | color(Color::RGB(0x7a, 0xa2, 0xf7)),
    separator(),
    selector->Render(),
}) | border | color(Color::RGB(0x7d, 0xca, 0xfe));

screen.Print();
```

### 2. Core Layer

#### Markdown Parser

**Location:** `src/core/parser.cpp`, `include/yaqeen/core/parser.hpp`

**Purpose:** Parse markdown files and extract project structure

**Algorithm:**
1. Parse markdown with md4c library
2. Extract code blocks
3. Identify tree structure
4. Build hierarchical node tree

**Data Structures:**
```cpp
namespace yaqeen::core {

enum class NodeType {
    Directory,
    File
};

struct Node {
    std::string name;
    NodeType type;
    std::vector<std::unique_ptr<Node>> children;
    Node* parent = nullptr;
    std::string content;  // For files

    bool is_directory() const { return type == NodeType::Directory; }
    bool is_file() const { return type == NodeType::File; }
};

class MarkdownParser {
public:
    Result<std::unique_ptr<Node>> parse_file(const std::filesystem::path& path);
    Result<std::unique_ptr<Node>> parse_string(std::string_view content);

private:
    std::unique_ptr<Node> build_tree(const std::vector<std::string>& lines);
    int calculate_depth(std::string_view line);
    std::string extract_name(std::string_view line);
    NodeType determine_type(std::string_view name);
};

} // namespace yaqeen::core
```

**Parsing Flow:**
```cpp
Result<std::unique_ptr<Node>> MarkdownParser::parse_file(const fs::path& path) {
    // 1. Read file
    std::ifstream file(path);
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    // 2. Parse markdown with md4c
    MD_PARSER parser = {/* callbacks */};
    md_parse(content.data(), content.size(), &parser, &ctx);

    // 3. Extract code block with tree structure
    auto tree_block = find_tree_block(ctx.code_blocks);
    if (!tree_block) {
        return Error("No tree structure found");
    }

    // 4. Build node tree
    auto root = build_tree(split_lines(*tree_block));

    return Ok(std::move(root));
}
```

#### Template Manager

**Location:** `src/core/template_manager.cpp`

**Purpose:** Load, validate, and manage project templates

**Responsibilities:**
- Load templates from JSON files
- Validate template schema
- Search template directories
- Cache loaded templates
- Resolve template dependencies

**Implementation:**
```cpp
class TemplateManager {
public:
    // Load template from file
    Result<Template> load_template(const std::filesystem::path& path);

    // List all available templates
    std::vector<TemplateMetadata> list_templates() const;

    // Find template by name
    Result<Template> find_template(std::string_view name) const;

    // Validate template structure
    Result<void> validate_template(const Template& tmpl) const;

private:
    // Template cache
    std::unordered_map<std::string, Template> template_cache_;

    // Search paths
    std::vector<std::filesystem::path> search_paths_;

    // Load templates from directory
    void scan_directory(const std::filesystem::path& dir);
};

struct Template {
    std::string name;
    std::string description;
    std::string version;
    std::string category;
    std::vector<std::string> tags;
    std::unique_ptr<Node> structure;
};
```

**Template Loading:**
```cpp
Result<Template> TemplateManager::load_template(const fs::path& path) {
    // 1. Read JSON file
    std::ifstream file(path);
    nlohmann::json j;
    file >> j;

    // 2. Validate schema
    if (!j.contains("name") || !j.contains("structure")) {
        return Error("Invalid template: missing required fields");
    }

    // 3. Parse template
    Template tmpl;
    tmpl.name = j["name"];
    tmpl.description = j.value("description", "");
    tmpl.version = j.value("version", "1.0.0");
    tmpl.category = j.value("category", "other");
    tmpl.tags = j.value("tags", std::vector<std::string>{});

    // 4. Build structure tree
    tmpl.structure = parse_structure(j["structure"]);

    // 5. Cache template
    template_cache_[tmpl.name] = tmpl;

    return Ok(tmpl);
}
```

#### Generator

**Location:** `src/core/generator.cpp`

**Purpose:** Generate project files and directories from template or parsed structure

**Algorithm:**
1. Validate output directory
2. Create directory structure (depth-first)
3. Write files with content
4. Apply template variables
5. Run post-generation hooks

**Implementation:**
```cpp
class Generator {
public:
    struct Options {
        bool dry_run = false;
        bool force = false;
        bool verbose = false;
        std::filesystem::path output_dir = ".";
    };

    struct Stats {
        size_t files_created = 0;
        size_t dirs_created = 0;
        std::chrono::milliseconds elapsed{0};
    };

    Result<Stats> generate(const Node& root, const Options& opts);

private:
    void create_directory(const std::filesystem::path& path);
    void create_file(const std::filesystem::path& path, std::string_view content);
    void traverse_tree(const Node& node, const std::filesystem::path& base);

    Options options_;
    Stats stats_;
};
```

**Generation Flow:**
```cpp
Result<Stats> Generator::generate(const Node& root, const Options& opts) {
    options_ = opts;

    auto start = std::chrono::steady_clock::now();

    // 1. Validate output directory
    if (fs::exists(opts.output_dir) && !opts.force) {
        return Error("Directory already exists");
    }

    // 2. Create root directory
    if (!opts.dry_run) {
        create_directory(opts.output_dir);
    }

    // 3. Traverse tree and create files/directories
    traverse_tree(root, opts.output_dir);

    // 4. Calculate statistics
    auto end = std::chrono::steady_clock::now();
    stats_.elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return Ok(stats_);
}

void Generator::traverse_tree(const Node& node, const fs::path& base) {
    fs::path current = base / node.name;

    if (node.is_directory()) {
        // Create directory
        if (!options_.dry_run) {
            create_directory(current);
        }
        stats_.dirs_created++;

        // Recurse into children
        for (const auto& child : node.children) {
            traverse_tree(*child, current);
        }
    } else {
        // Create file
        if (!options_.dry_run) {
            create_file(current, node.content);
        }
        stats_.files_created++;
    }

    if (options_.verbose) {
        logger::info("Created: {}", current.string());
    }
}
```

### 3. Utility Layer

#### Logger

**Location:** `src/utils/logger.cpp`

**Purpose:** Centralized logging with multiple levels and outputs

**Features:**
- Log levels: debug, info, warning, error
- Colored output
- File logging
- Structured logging (JSON format)

**Implementation:**
```cpp
namespace yaqeen::utils {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class Logger {
public:
    static void set_level(LogLevel level);
    static void set_output_file(const std::filesystem::path& path);
    static void set_format(Format format);  // Pretty, JSON, Compact

    template<typename... Args>
    static void debug(fmt::format_string<Args...> fmt, Args&&... args);

    template<typename... Args>
    static void info(fmt::format_string<Args...> fmt, Args&&... args);

    template<typename... Args>
    static void warning(fmt::format_string<Args...> fmt, Args&&... args);

    template<typename... Args>
    static void error(fmt::format_string<Args...> fmt, Args&&... args);

private:
    static LogLevel current_level_;
    static std::optional<std::ofstream> output_file_;
    static Format format_;
};

} // namespace yaqeen::utils
```

#### Error Handling

**Location:** `src/utils/error.cpp`

**Purpose:** Result type for error propagation without exceptions

**Implementation:**
```cpp
template<typename T>
class Result {
public:
    // Success
    static Result Ok(T value) {
        return Result(std::move(value));
    }

    // Error
    static Result Error(std::string message) {
        return Result(std::move(message));
    }

    bool is_ok() const { return has_value_; }
    bool is_error() const { return !has_value_; }

    T& value() {
        if (!has_value_) throw std::runtime_error("Result is error");
        return value_;
    }

    const std::string& error() const {
        if (has_value_) throw std::runtime_error("Result is ok");
        return error_;
    }

    // Monadic operations
    template<typename F>
    auto map(F&& func) -> Result<decltype(func(value_))>;

    template<typename F>
    auto and_then(F&& func) -> decltype(func(value_));

private:
    Result(T value) : has_value_(true), value_(std::move(value)) {}
    Result(std::string error) : has_value_(false), error_(std::move(error)) {}

    bool has_value_;
    T value_;
    std::string error_;
};
```

**Usage:**
```cpp
Result<Template> load_template(const fs::path& path) {
    if (!fs::exists(path)) {
        return Result<Template>::Error("Template file not found");
    }

    // ... load template ...

    return Result<Template>::Ok(template);
}

// Chain results
auto result = load_template("template.json")
    .and_then([](const Template& tmpl) {
        return validate_template(tmpl);
    })
    .map([](const Template& tmpl) {
        return generate_project(tmpl);
    });

if (result.is_error()) {
    logger::error("Failed: {}", result.error());
}
```

#### Validators

**Location:** `src/utils/validators.cpp`

**Purpose:** Validation functions for inputs

**Functions:**
```cpp
namespace yaqeen::utils {

// Validate project name
Result<void> validate_project_name(std::string_view name);

// Validate template name
Result<void> validate_template_name(std::string_view name);

// Validate file path
Result<void> validate_path(const std::filesystem::path& path);

// Validate JSON structure
Result<void> validate_json_schema(const nlohmann::json& j, const nlohmann::json& schema);

} // namespace yaqeen::utils
```

## Design Patterns

### 1. Builder Pattern

Used for complex object construction:

```cpp
auto generator = Generator::Builder()
    .output_dir("./output")
    .dry_run(false)
    .verbose(true)
    .force(false)
    .build();
```

### 2. Strategy Pattern

Different parsing strategies:

```cpp
class ParsingStrategy {
public:
    virtual Result<Node> parse(std::string_view content) = 0;
};

class MarkdownStrategy : public ParsingStrategy { /* ... */ };
class JSONStrategy : public ParsingStrategy { /* ... */ };

class Parser {
    void set_strategy(std::unique_ptr<ParsingStrategy> strategy);
};
```

### 3. Observer Pattern

For progress tracking:

```cpp
class ProgressObserver {
public:
    virtual void on_file_created(const std::filesystem::path& path) = 0;
    virtual void on_directory_created(const std::filesystem::path& path) = 0;
    virtual void on_complete(const Stats& stats) = 0;
};

class Generator {
    void add_observer(std::shared_ptr<ProgressObserver> observer);
    void notify_file_created(const fs::path& path);
};
```

### 4. Factory Pattern

Template creation:

```cpp
class TemplateFactory {
public:
    static std::unique_ptr<Template> create(const std::string& type);
};

auto tmpl = TemplateFactory::create("web");
```

## Performance Optimizations

### 1. Memory Efficiency

**Small String Optimization:**
```cpp
// Use string_view to avoid copies
void process(std::string_view text);

// Use move semantics
std::unique_ptr<Node> create_node(std::string name) {
    return std::make_unique<Node>(std::move(name));
}
```

**Memory Pooling:**
```cpp
class NodeAllocator {
    std::vector<std::unique_ptr<Node>> pool_;

public:
    Node* allocate() {
        if (pool_.empty()) {
            return new Node();
        }
        auto node = std::move(pool_.back());
        pool_.pop_back();
        return node.release();
    }

    void deallocate(Node* node) {
        pool_.emplace_back(node);
    }
};
```

### 2. I/O Optimization

**Buffered Writing:**
```cpp
class BufferedWriter {
    static constexpr size_t BUFFER_SIZE = 8192;
    char buffer_[BUFFER_SIZE];
    size_t pos_ = 0;
    std::ofstream file_;

public:
    void write(std::string_view data) {
        if (pos_ + data.size() > BUFFER_SIZE) {
            flush();
        }
        std::memcpy(buffer_ + pos_, data.data(), data.size());
        pos_ += data.size();
    }

    void flush() {
        file_.write(buffer_, pos_);
        pos_ = 0;
    }
};
```

**Parallel File Creation:**
```cpp
#include <thread>
#include <future>

std::vector<std::future<void>> futures;

for (const auto& file : files) {
    futures.push_back(std::async(std::launch::async, [&]() {
        create_file(file);
    }));
}

for (auto& future : futures) {
    future.wait();
}
```

### 3. Template Caching

```cpp
class TemplateCache {
    std::unordered_map<std::string, Template> cache_;
    std::mutex mutex_;

public:
    std::optional<Template> get(const std::string& name) {
        std::lock_guard lock(mutex_);
        auto it = cache_.find(name);
        return it != cache_.end() ? std::make_optional(it->second) : std::nullopt;
    }

    void put(const std::string& name, Template tmpl) {
        std::lock_guard lock(mutex_);
        cache_[name] = std::move(tmpl);
    }
};
```

## Testing Architecture

### Unit Tests

```cpp
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Parser extracts tree structure", "[parser]") {
    std::string markdown = R"(
# Project

```
myproject/
├── src/
└── README.md
```
    )";

    MarkdownParser parser;
    auto result = parser.parse_string(markdown);

    REQUIRE(result.is_ok());
    auto root = std::move(result.value());
    REQUIRE(root->name == "myproject");
    REQUIRE(root->children.size() == 2);
}
```

### Integration Tests

```cpp
TEST_CASE("End-to-end project generation", "[integration]") {
    // Load template
    TemplateManager manager;
    auto tmpl_result = manager.load_template("react-typescript.json");
    REQUIRE(tmpl_result.is_ok());

    // Generate project
    Generator generator;
    Generator::Options opts;
    opts.output_dir = "./test-output";
    opts.dry_run = false;

    auto gen_result = generator.generate(*tmpl_result.value().structure, opts);
    REQUIRE(gen_result.is_ok());

    // Verify files created
    REQUIRE(fs::exists("./test-output/src"));
    REQUIRE(fs::exists("./test-output/package.json"));
}
```

## Build System

### CMake Structure

```cmake
# Project
cmake_minimum_required(VERSION 3.14)
project(yaqeen VERSION 1.0.0 LANGUAGES CXX)

# C++17 required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sources
set(YAQEEN_SOURCES
    src/main.cpp
    src/core/parser.cpp
    src/core/generator.cpp
    src/core/template_manager.cpp
    src/ui/animations.cpp
    src/utils/logger.cpp
)

# Executable
add_executable(yaqeen ${YAQEEN_SOURCES})

# Include directories
target_include_directories(yaqeen PRIVATE include)

# Link libraries
target_link_libraries(yaqeen PRIVATE
    CLI11::CLI11
    ftxui::screen ftxui::dom ftxui::component
    nlohmann_json::nlohmann_json
    md4c
)

# Tests
if(BUILD_TESTS)
    add_executable(yaqeen_tests tests/test_parser.cpp)
    target_link_libraries(yaqeen_tests PRIVATE Catch2::Catch2WithMain)
endif()
```

## Summary

Yaqeen's architecture is designed for:
- ✅ **High Performance** - Fast project generation (< 200ms for typical projects)
- ✅ **Modularity** - Clean separation between CLI, core, and utilities
- ✅ **Extensibility** - Easy to add new features and templates
- ✅ **Reliability** - Result types for error handling, comprehensive testing
- ✅ **Maintainability** - Clear code structure, design patterns

Key components:
- **Parser** - Markdown/JSON parsing with md4c and nlohmann/json
- **Template Manager** - Template loading, caching, and management
- **Generator** - File system operations with parallel creation
- **UI** - Beautiful terminal UI with FTXUI
- **Utilities** - Logging, error handling, validation

Next steps:
- Explore [Contributing](contributing.md)
- Read [Building from Source](building.md)
- Check [API Documentation](../api/library-usage.md)
