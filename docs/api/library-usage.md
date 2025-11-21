# Library Usage

Yaqeen can be used as a C++ library in your own applications to programmatically generate project structures. This guide covers the public API and integration methods.

## Installation

### As a Library

#### CMake FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
    yaqeen
    GIT_REPOSITORY https://github.com/yourusername/yaqeen.git
    GIT_TAG        v1.0.0
)

FetchContent_MakeAvailable(yaqeen)

target_link_libraries(your_app PRIVATE yaqeen::yaqeen)
```

#### CMake find_package

```cmake
find_package(yaqeen REQUIRED)

target_link_libraries(your_app PRIVATE yaqeen::yaqeen)
```

#### Manual Installation

```bash
git clone https://github.com/yourusername/yaqeen.git
cd yaqeen
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON ..
make
sudo make install
```

## Basic Usage

### Hello World

```cpp
#include <yaqeen/yaqeen.hpp>
#include <iostream>

int main() {
    using namespace yaqeen;

    // Create a simple project structure
    auto result = Generator::quick_create(
        "my-project",
        {
            {"src/", {}},
            {"src/main.cpp", "int main() { return 0; }"},
            {"README.md", "# My Project"}
        }
    );

    if (result.is_ok()) {
        std::cout << "Project created successfully!\n";
    } else {
        std::cerr << "Error: " << result.error() << "\n";
    }

    return 0;
}
```

Compile and run:
```bash
g++ -std=c++17 example.cpp -lyaqeen -o example
./example
```

## Core API

### Namespace Structure

```cpp
namespace yaqeen {
    namespace core {
        class Parser;
        class Generator;
        class TemplateManager;
        struct Node;
    }

    namespace utils {
        class Logger;
        template<typename T> class Result;
        namespace validators { /* ... */ }
    }

    namespace ui {
        class ProgressTracker;
        class Theme;
    }
}
```

## Parser API

### MarkdownParser

Parse markdown files to extract project structures.

**Header:**
```cpp
#include <yaqeen/core/parser.hpp>
```

**Class Definition:**
```cpp
namespace yaqeen::core {

class MarkdownParser {
public:
    // Parse markdown file
    Result<std::unique_ptr<Node>> parse_file(
        const std::filesystem::path& path
    );

    // Parse markdown string
    Result<std::unique_ptr<Node>> parse_string(
        std::string_view content
    );

    // Configuration
    struct Options {
        bool allow_unicode = true;
        bool strict_indentation = false;
        size_t max_depth = 256;
    };

    void set_options(const Options& opts);
    const Options& get_options() const;
};

} // namespace yaqeen::core
```

**Example:**
```cpp
#include <yaqeen/core/parser.hpp>
#include <fstream>
#include <iostream>

using namespace yaqeen::core;

int main() {
    MarkdownParser parser;

    // Configure parser
    MarkdownParser::Options opts;
    opts.strict_indentation = true;
    opts.max_depth = 128;
    parser.set_options(opts);

    // Parse file
    auto result = parser.parse_file("structure.md");

    if (result.is_ok()) {
        auto root = std::move(result.value());
        std::cout << "Parsed successfully!\n";
        std::cout << "Root: " << root->name << "\n";
        std::cout << "Children: " << root->children.size() << "\n";
    } else {
        std::cerr << "Parse error: " << result.error() << "\n";
        return 1;
    }

    return 0;
}
```

### Node Structure

**Definition:**
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
    Node* parent;
    std::string content;  // File content

    // Constructors
    Node(std::string name, NodeType type);
    Node(const Node&) = delete;  // No copy
    Node(Node&&) = default;       // Move only

    // Type checking
    bool is_directory() const;
    bool is_file() const;

    // Tree operations
    void add_child(std::unique_ptr<Node> child);
    Node* find_child(std::string_view name);
    size_t depth() const;
    size_t total_files() const;
    size_t total_directories() const;

    // Traversal
    void traverse(std::function<void(const Node&)> callback) const;
    void traverse_files(std::function<void(const Node&)> callback) const;
    void traverse_directories(std::function<void(const Node&)> callback) const;
};

} // namespace yaqeen::core
```

**Example:**
```cpp
// Create node tree manually
auto root = std::make_unique<Node>("myproject", NodeType::Directory);

auto src = std::make_unique<Node>("src", NodeType::Directory);
src->add_child(std::make_unique<Node>("main.cpp", NodeType::File));
src->add_child(std::make_unique<Node>("utils.cpp", NodeType::File));

root->add_child(std::move(src));
root->add_child(std::make_unique<Node>("README.md", NodeType::File));

// Traverse tree
root->traverse([](const Node& node) {
    std::cout << node.name << " ("
              << (node.is_directory() ? "dir" : "file") << ")\n";
});

// Count files
std::cout << "Total files: " << root->total_files() << "\n";
std::cout << "Total directories: " << root->total_directories() << "\n";
```

## Generator API

### Generator

Generate project files and directories from node tree.

**Header:**
```cpp
#include <yaqeen/core/generator.hpp>
```

**Class Definition:**
```cpp
namespace yaqeen::core {

class Generator {
public:
    // Configuration
    struct Options {
        std::filesystem::path output_dir = ".";
        bool dry_run = false;
        bool force = false;
        bool verbose = false;
        bool parallel = true;
        size_t max_threads = 4;
        std::map<std::string, std::string> variables;
    };

    // Statistics
    struct Stats {
        size_t files_created = 0;
        size_t dirs_created = 0;
        size_t bytes_written = 0;
        std::chrono::milliseconds elapsed{0};
    };

    // Constructor
    explicit Generator(const Options& opts = {});

    // Generate from node tree
    Result<Stats> generate(const Node& root);

    // Generate from template
    Result<Stats> generate_from_template(
        const Template& tmpl,
        std::string_view project_name
    );

    // Quick create (static helper)
    static Result<Stats> quick_create(
        std::string_view project_name,
        const std::map<std::string, std::string>& structure
    );

    // Progress callbacks
    using ProgressCallback = std::function<void(
        const std::filesystem::path&,
        float progress
    )>;

    void set_progress_callback(ProgressCallback callback);

private:
    Options options_;
    ProgressCallback progress_callback_;
};

} // namespace yaqeen::core
```

**Example:**
```cpp
#include <yaqeen/core/generator.hpp>
#include <yaqeen/core/parser.hpp>

using namespace yaqeen::core;

int main() {
    // Parse markdown
    MarkdownParser parser;
    auto parse_result = parser.parse_file("structure.md");

    if (parse_result.is_error()) {
        std::cerr << "Parse error: " << parse_result.error() << "\n";
        return 1;
    }

    auto root = std::move(parse_result.value());

    // Configure generator
    Generator::Options opts;
    opts.output_dir = "./output";
    opts.verbose = true;
    opts.parallel = true;
    opts.variables = {
        {"project_name", "MyApp"},
        {"author", "John Doe"},
        {"year", "2024"}
    };

    Generator generator(opts);

    // Set progress callback
    generator.set_progress_callback([](const auto& path, float progress) {
        std::cout << "Creating: " << path << " ("
                  << (progress * 100) << "%)\n";
    });

    // Generate project
    auto gen_result = generator.generate(*root);

    if (gen_result.is_ok()) {
        const auto& stats = gen_result.value();
        std::cout << "Success!\n";
        std::cout << "Files: " << stats.files_created << "\n";
        std::cout << "Directories: " << stats.dirs_created << "\n";
        std::cout << "Time: " << stats.elapsed.count() << "ms\n";
    } else {
        std::cerr << "Generation error: " << gen_result.error() << "\n";
        return 1;
    }

    return 0;
}
```

## Template Manager API

### TemplateManager

Load and manage project templates.

**Header:**
```cpp
#include <yaqeen/core/template_manager.hpp>
```

**Class Definition:**
```cpp
namespace yaqeen::core {

struct TemplateMetadata {
    std::string name;
    std::string description;
    std::string version;
    std::string category;
    std::vector<std::string> tags;
    std::filesystem::path path;
};

struct Template {
    TemplateMetadata metadata;
    std::unique_ptr<Node> structure;
    std::map<std::string, std::string> file_contents;
};

class TemplateManager {
public:
    // Constructor
    TemplateManager();
    explicit TemplateManager(std::vector<std::filesystem::path> search_paths);

    // Load template
    Result<Template> load_template(std::string_view name);
    Result<Template> load_template_from_file(const std::filesystem::path& path);

    // List templates
    std::vector<TemplateMetadata> list_templates() const;
    std::vector<TemplateMetadata> list_templates_by_category(std::string_view category) const;
    std::vector<TemplateMetadata> search_templates(std::string_view query) const;

    // Validation
    Result<void> validate_template(const Template& tmpl) const;

    // Search paths
    void add_search_path(const std::filesystem::path& path);
    const std::vector<std::filesystem::path>& get_search_paths() const;

    // Cache management
    void clear_cache();
    size_t cache_size() const;

private:
    std::vector<std::filesystem::path> search_paths_;
    std::unordered_map<std::string, Template> cache_;
};

} // namespace yaqeen::core
```

**Example:**
```cpp
#include <yaqeen/core/template_manager.hpp>
#include <yaqeen/core/generator.hpp>

using namespace yaqeen::core;

int main() {
    // Create template manager
    TemplateManager manager({
        "/usr/local/share/yaqeen/templates",
        "~/.yaqeen/templates",
        "./templates"
    });

    // List all templates
    auto templates = manager.list_templates();
    std::cout << "Available templates:\n";
    for (const auto& tmpl : templates) {
        std::cout << "  " << tmpl.name << " - " << tmpl.description << "\n";
    }

    // Load specific template
    auto load_result = manager.load_template("react-typescript");

    if (load_result.is_error()) {
        std::cerr << "Failed to load template: " << load_result.error() << "\n";
        return 1;
    }

    auto tmpl = std::move(load_result.value());

    // Validate template
    auto validation = manager.validate_template(tmpl);
    if (validation.is_error()) {
        std::cerr << "Invalid template: " << validation.error() << "\n";
        return 1;
    }

    // Generate project from template
    Generator generator;
    auto gen_result = generator.generate_from_template(tmpl, "my-app");

    if (gen_result.is_ok()) {
        std::cout << "Project created successfully!\n";
    }

    return 0;
}
```

## Result Type

### Result<T>

Type-safe error handling without exceptions.

**Header:**
```cpp
#include <yaqeen/utils/result.hpp>
```

**Definition:**
```cpp
namespace yaqeen::utils {

template<typename T>
class Result {
public:
    // Create success result
    static Result Ok(T value);

    // Create error result
    static Result Error(std::string message);

    // Check status
    bool is_ok() const;
    bool is_error() const;

    // Access value (throws if error)
    T& value();
    const T& value() const;
    T&& take_value();

    // Access error (throws if ok)
    const std::string& error() const;

    // Safe access
    T value_or(T default_value) const;
    T* value_ptr();
    const T* value_ptr() const;

    // Monadic operations
    template<typename F>
    auto map(F&& func) -> Result<decltype(func(value()))>;

    template<typename F>
    auto and_then(F&& func) -> decltype(func(value()));

    template<typename F>
    Result or_else(F&& func) const;
};

// Specialization for void
template<>
class Result<void> {
public:
    static Result Ok();
    static Result Error(std::string message);

    bool is_ok() const;
    bool is_error() const;
    const std::string& error() const;
};

} // namespace yaqeen::utils
```

**Example:**
```cpp
#include <yaqeen/utils/result.hpp>

using namespace yaqeen::utils;

// Function returning Result
Result<int> divide(int a, int b) {
    if (b == 0) {
        return Result<int>::Error("Division by zero");
    }
    return Result<int>::Ok(a / b);
}

int main() {
    auto result = divide(10, 2);

    if (result.is_ok()) {
        std::cout << "Result: " << result.value() << "\n";
    } else {
        std::cerr << "Error: " << result.error() << "\n";
    }

    // Monadic operations
    auto chained = divide(20, 2)
        .map([](int x) { return x * 2; })
        .and_then([](int x) { return divide(x, 3); });

    std::cout << "Chained result: " << chained.value_or(0) << "\n";

    return 0;
}
```

## Utilities

### Logger

**Header:**
```cpp
#include <yaqeen/utils/logger.hpp>
```

**API:**
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
    static void enable_colors(bool enable);
    static void set_output_file(const std::filesystem::path& path);

    template<typename... Args>
    static void debug(fmt::format_string<Args...> fmt, Args&&... args);

    template<typename... Args>
    static void info(fmt::format_string<Args...> fmt, Args&&... args);

    template<typename... Args>
    static void warning(fmt::format_string<Args...> fmt, Args&&... args);

    template<typename... Args>
    static void error(fmt::format_string<Args...> fmt, Args&&... args);
};

} // namespace yaqeen::utils
```

**Example:**
```cpp
#include <yaqeen/utils/logger.hpp>

using namespace yaqeen::utils;

int main() {
    // Configure logger
    Logger::set_level(LogLevel::Debug);
    Logger::enable_colors(true);

    // Log messages
    Logger::debug("Starting application");
    Logger::info("Processing file: {}", "test.txt");
    Logger::warning("Deprecated function used");
    Logger::error("Failed to open file: {}", "missing.txt");

    return 0;
}
```

## Complete Example

### Project Generator Application

```cpp
#include <yaqeen/yaqeen.hpp>
#include <iostream>
#include <string>

using namespace yaqeen;

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <template> <project-name>\n";
        return 1;
    }

    std::string template_name = argv[1];
    std::string project_name = argv[2];

    // Configure logger
    utils::Logger::set_level(utils::LogLevel::Info);
    utils::Logger::enable_colors(true);

    // Create template manager
    core::TemplateManager manager;

    // Load template
    utils::Logger::info("Loading template: {}", template_name);
    auto load_result = manager.load_template(template_name);

    if (load_result.is_error()) {
        utils::Logger::error("Failed to load template: {}", load_result.error());
        return 1;
    }

    auto tmpl = std::move(load_result.value());

    // Configure generator
    core::Generator::Options opts;
    opts.output_dir = "./" + project_name;
    opts.verbose = true;
    opts.variables = {
        {"project_name", project_name},
        {"author", "User"},
        {"year", "2024"}
    };

    core::Generator generator(opts);

    // Set progress callback
    generator.set_progress_callback([](const auto& path, float progress) {
        std::cout << "\r[" << std::string(static_cast<int>(progress * 50), '=')
                  << std::string(50 - static_cast<int>(progress * 50), ' ')
                  << "] " << (progress * 100) << "%";
        std::cout.flush();
    });

    // Generate project
    utils::Logger::info("Generating project...");
    auto gen_result = generator.generate_from_template(tmpl, project_name);

    std::cout << "\n";

    if (gen_result.is_error()) {
        utils::Logger::error("Generation failed: {}", gen_result.error());
        return 1;
    }

    const auto& stats = gen_result.value();
    utils::Logger::info("✓ Project created successfully!");
    utils::Logger::info("  Files: {}", stats.files_created);
    utils::Logger::info("  Directories: {}", stats.dirs_created);
    utils::Logger::info("  Time: {}ms", stats.elapsed.count());

    return 0;
}
```

Compile:
```bash
g++ -std=c++17 generator.cpp -lyaqeen -o generator
```

Use:
```bash
./generator react-typescript my-app
```

## API Reference Summary

| Component | Header | Purpose |
|-----------|--------|---------|
| `MarkdownParser` | `<yaqeen/core/parser.hpp>` | Parse markdown structures |
| `Generator` | `<yaqeen/core/generator.hpp>` | Generate files/directories |
| `TemplateManager` | `<yaqeen/core/template_manager.hpp>` | Manage templates |
| `Node` | `<yaqeen/core/node.hpp>` | Tree node structure |
| `Result<T>` | `<yaqeen/utils/result.hpp>` | Error handling |
| `Logger` | `<yaqeen/utils/logger.hpp>` | Logging utilities |

## Best Practices

### 1. Always Check Results

```cpp
auto result = generator.generate(*root);
if (result.is_error()) {
    // Handle error
    return 1;
}
// Use result.value()
```

### 2. Use RAII for Resources

```cpp
class Generator {
    std::ofstream log_file_;  // Automatically closed
public:
    Generator(const std::string& log_path)
        : log_file_(log_path) {}
    ~Generator() = default;  // log_file_ closed automatically
};
```

### 3. Leverage Move Semantics

```cpp
auto root = parser.parse_file("structure.md").value();
generator.generate(std::move(root));  // Move, don't copy
```

### 4. Handle Progress Updates

```cpp
generator.set_progress_callback([](const auto& path, float progress) {
    // Update UI or log progress
});
```

## Summary

Key points:
- ✅ Use Yaqeen as a C++ library in your projects
- ✅ CMake integration with FetchContent or find_package
- ✅ Type-safe Result<T> for error handling
- ✅ Comprehensive API for parsing, generation, and templates
- ✅ Progress callbacks for UI updates
- ✅ Modern C++17 features (move semantics, smart pointers)

Next steps:
- Read [Architecture Documentation](../development/architecture.md)
- Explore [Contributing Guide](../development/contributing.md)
- Check example projects
