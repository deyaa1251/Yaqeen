# Contributing to Yaqeen

Thank you for your interest in contributing to Yaqeen! This document provides guidelines and instructions for contributing.

## Code of Conduct

By participating in this project, you agree to maintain a respectful and inclusive environment for everyone.

## Ways to Contribute

- **Report bugs** - Open an issue with details
- **Suggest features** - Share your ideas
- **Improve documentation** - Fix typos, add examples
- **Submit templates** - Create new project templates
- **Fix bugs** - Submit pull requests
- **Write tests** - Improve test coverage

## Getting Started

### 1. Fork and Clone

```bash
# Fork on GitHub, then clone your fork
git clone https://github.com/YOUR_USERNAME/yaqeen.git
cd yaqeen

# Add upstream remote
git remote add upstream https://github.com/yourusername/yaqeen.git
```

### 2. Set Up Development Environment

**Requirements:**
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14+
- Git

**Build:**
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

### 3. Create a Branch

```bash
git checkout -b feature/your-feature-name
# or
git checkout -b fix/bug-description
```

## Development Workflow

### Building

```bash
# Debug build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### Running Tests

```bash
cd build
ctest --output-on-failure

# Or run specific test
./yaqeen_tests
```

### Code Style

We follow modern C++ best practices:

- **C++ Standard**: C++17
- **Naming**:
  - Classes: `PascalCase`
  - Functions: `snake_case`
  - Variables: `snake_case`
  - Constants: `UPPER_CASE`
  - Namespaces: `lowercase`

- **Formatting**:
  - Indent: 4 spaces
  - Max line length: 100 characters
  - Braces: Same line for control structures

**Example:**
```cpp
namespace yaqeen::core {

class FileGenerator {
public:
    struct Options {
        bool dry_run = false;
        bool verbose = false;
    };

    Result<Stats> generate(
        const Node& root,
        const std::filesystem::path& output_dir
    );

private:
    void process_node(const Node& node);
};

} // namespace yaqeen::core
```

### Commit Messages

Follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:

```
type(scope): subject

body (optional)

footer (optional)
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

**Examples:**
```
feat(templates): add Svelte template

fix(parser): handle empty markdown files correctly

docs(readme): update installation instructions

test(generator): add tests for file creation
```

## Contributing Templates

### Template Guidelines

1. **Use clear naming** - Use kebab-case (e.g., `react-typescript`)
2. **Provide description** - Clear, concise description
3. **Add appropriate tags** - Help users find your template
4. **Follow best practices** - Use industry-standard structure
5. **Include README** - Document usage and setup

### Template Format

```json
{
  "name": "template-name",
  "description": "Brief description of the template",
  "version": "1.0.0",
  "category": "web|backend|mobile|patterns|languages|other",
  "tags": ["tag1", "tag2"],
  "author": "Your Name (optional)",
  "structure": {
    "src/": {
      "main.ts": "",
      "utils/": {}
    },
    "tests/": {},
    "package.json": "",
    "README.md": ""
  }
}
```

### Submitting a Template

1. Create template JSON in appropriate `templates/` subdirectory
2. Test the template:
   ```bash
   yaqeen create --template your-template --name test-project
   ```
3. Add documentation to `docs/templates/`
4. Submit pull request

## Pull Request Process

### 1. Ensure Quality

- Code compiles without warnings
- All tests pass
- New features have tests
- Documentation is updated
- Code follows style guidelines

### 2. Update Documentation

- Update README if needed
- Add/update API documentation
- Update CHANGELOG.md

### 3. Submit PR

- Push your branch to your fork
- Open a pull request on GitHub
- Fill out the PR template
- Link related issues

### 4. Code Review

- Address reviewer feedback
- Make requested changes
- Keep commits clean and logical

### 5. Merge

Once approved, a maintainer will merge your PR.

## Testing Guidelines

### Writing Tests

Use Catch2 framework:

```cpp
#include <catch2/catch_test_macros.hpp>
#include "yaqeen/core/parser.hpp"

TEST_CASE("Parser handles valid input", "[parser]") {
    MarkdownParser parser;
    auto result = parser.parse_string("...");

    REQUIRE(result.is_ok());
    REQUIRE(result.value() != nullptr);
}

TEST_CASE("Parser rejects invalid input", "[parser]") {
    MarkdownParser parser;
    auto result = parser.parse_string("");

    REQUIRE(result.is_error());
}
```

### Test Categories

- **Unit tests** - Test individual components
- **Integration tests** - Test component interactions
- **Template tests** - Verify templates work correctly

### Running Tests

```bash
# All tests
ctest

# Specific test
./yaqeen_tests "[parser]"

# Verbose output
ctest --output-on-failure --verbose
```

## Documentation

### Documentation Structure

- `/docs` - GitBook documentation
- `README.md` - Main project README
- Code comments - In-code documentation

### Writing Documentation

- Use clear, concise language
- Provide examples
- Keep it up to date
- Use proper markdown formatting

## Getting Help

- **Discussions** - Ask questions on GitHub Discussions
- **Issues** - Report bugs or request features
- **Discord** - Join our community (coming soon)

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Recognition

Contributors will be recognized in:
- CONTRIBUTORS.md
- Release notes
- GitHub contributors page

Thank you for contributing to Yaqeen!
