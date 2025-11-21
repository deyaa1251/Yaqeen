#include <catch2/catch_test_macros.hpp>
#include "yaqeen/core/parser.hpp"

using namespace yaqeen::core;

TEST_CASE("MarkdownParser can parse simple tree structure", "[parser]") {
    std::string markdown = R"(
# Project Structure

```
root/
├── src/
│   ├── main.cpp
│   └── utils/
└── include/
```
)";

    MarkdownParser parser;
    auto result = parser.parse_string(markdown);

    REQUIRE(result.is_ok());

    auto& root = result.value();
    REQUIRE(root != nullptr);
    REQUIRE(root->name == "root");
    REQUIRE(root->is_directory());
    REQUIRE(root->children.size() > 0);
}

TEST_CASE("MarkdownParser handles files correctly", "[parser]") {
    std::string markdown = R"(
```
project/
├── file.txt
└── README.md
```
)";

    MarkdownParser parser;
    auto result = parser.parse_string(markdown);

    REQUIRE(result.is_ok());

    auto& root = result.value();
    REQUIRE(root != nullptr);

    // Check for files in the tree
    bool hasFiles = false;
    for (const auto& child : root->children) {
        if (child->is_file()) {
            hasFiles = true;
            break;
        }
    }

    REQUIRE(hasFiles);
}

TEST_CASE("MarkdownParser handles nested directories", "[parser]") {
    std::string markdown = R"(
```
root/
└── level1/
    └── level2/
        └── level3/
```
)";

    MarkdownParser parser;
    auto result = parser.parse_string(markdown);

    REQUIRE(result.is_ok());

    auto& root = result.value();
    REQUIRE(root != nullptr);

    // Should have nested structure
    REQUIRE(root->children.size() > 0);
}

TEST_CASE("TreeVisualizer generates correct output", "[parser]") {
    auto root = std::make_unique<Node>(Node::Type::Directory, "root");
    auto child1 = std::make_unique<Node>(Node::Type::Directory, "src");
    auto child2 = std::make_unique<Node>(Node::Type::File, "main.cpp");

    child1->add_child(std::move(child2));
    root->add_child(std::move(child1));

    std::string output = TreeVisualizer::visualize(*root, true);

    REQUIRE(!output.empty());
    REQUIRE(output.find("root") != std::string::npos);
    REQUIRE(output.find("src") != std::string::npos);
    REQUIRE(output.find("main.cpp") != std::string::npos);
}
