#include "yaqeen/core/parser.hpp"
#include "yaqeen/utils/validators.hpp"
#include "yaqeen/utils/logger.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <iostream>

namespace yaqeen::core {

// Node implementation
void Node::add_child(std::unique_ptr<Node> child) {
    children.push_back(std::move(child));
}

Node* Node::find_child(const std::string& name) {
    auto it = std::find_if(children.begin(), children.end(),
        [&name](const std::unique_ptr<Node>& node) {
            return node->name == name;
        });
    return it != children.end() ? it->get() : nullptr;
}

const Node* Node::find_child(const std::string& name) const {
    auto it = std::find_if(children.begin(), children.end(),
        [&name](const std::unique_ptr<Node>& node) {
            return node->name == name;
        });
    return it != children.end() ? it->get() : nullptr;
}

std::string Node::to_string(int indent) const {
    std::string result;
    std::string prefix(indent * 2, ' ');
    result += prefix + name;
    if (is_directory()) {
        result += "/";
    }
    result += "\n";

    for (const auto& child : children) {
        result += child->to_string(indent + 1);
    }

    return result;
}

// MarkdownParser implementation
struct MarkdownParser::Impl {
    // Implementation details
};

MarkdownParser::MarkdownParser() : impl_(std::make_unique<Impl>()) {
}

MarkdownParser::~MarkdownParser() = default;

Result<std::unique_ptr<Node>> MarkdownParser::parse(const std::filesystem::path& md_file) {
    // Validate file exists and is readable
    auto validation = Validator::validate_file_readable(md_file);
    if (validation.is_error()) {
        return validation.error();
    }

    // Read file content
    std::ifstream file(md_file);
    if (!file) {
        return Error(ErrorCode::FileNotFound, "Cannot open markdown file: " + md_file.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    return parse_string(content);
}

Result<std::unique_ptr<Node>> MarkdownParser::parse_string(const std::string& markdown) {
    LOG_DEBUG("Parsing markdown structure");

    // Extract tree structure from markdown
    return parse_tree_structure(markdown);
}

Result<std::unique_ptr<Node>> MarkdownParser::parse_tree_structure(const std::string& content) {
    // Split into lines
    std::vector<std::string> lines;
    std::istringstream stream(content);
    std::string line;

    bool in_code_block = false;
    std::vector<std::string> tree_lines;

    while (std::getline(stream, line)) {
        // Check for code block markers
        if (line.find("```") != std::string::npos) {
            in_code_block = !in_code_block;
            continue;
        }

        // Collect lines that look like tree structure
        if (in_code_block || is_tree_line(line)) {
            tree_lines.push_back(line);
        }
    }

    if (tree_lines.empty()) {
        return Error(ErrorCode::InvalidMarkdownFormat, "No tree structure found in markdown");
    }

    return parse_tree_lines(tree_lines);
}

std::unique_ptr<Node> MarkdownParser::parse_tree_lines(const std::vector<std::string>& lines) {
    auto root = std::make_unique<Node>(Node::Type::Directory, "root");
    std::stack<std::pair<int, Node*>> node_stack;
    node_stack.push({-1, root.get()});

    for (const auto& line : lines) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        auto line_info = parse_line(line);
        if (!line_info.has_value()) {
            continue;
        }

        // Pop nodes from stack until we find the parent
        while (!node_stack.empty() && node_stack.top().first >= line_info->indent_level) {
            node_stack.pop();
        }

        if (node_stack.empty()) {
            LOG_WARN("Invalid tree structure: no parent found");
            continue;
        }

        Node* parent = node_stack.top().second;

        // Create new node
        auto node_type = line_info->is_directory ? Node::Type::Directory : Node::Type::File;
        auto new_node = std::make_unique<Node>(node_type, line_info->name);
        Node* new_node_ptr = new_node.get();

        parent->add_child(std::move(new_node));
        node_stack.push({line_info->indent_level, new_node_ptr});
    }

    return root;
}

std::optional<MarkdownParser::LineInfo> MarkdownParser::parse_line(const std::string& line) {
    int indent = calculate_indent(line);

    bool is_directory = false;
    std::string name = extract_name(line, is_directory);

    if (name.empty()) {
        return std::nullopt;
    }

    LineInfo info;
    info.indent_level = indent;
    info.name = name;
    info.is_directory = is_directory;

    return info;
}

int MarkdownParser::calculate_indent(const std::string& line) {
    // Count leading spaces, considering tree characters
    int indent = 0;
    for (char c : line) {
        if (c == ' ' || c == '\t' || c == '|' || c == '├' || c == '└' ||
            c == '─' || c == '│' || c == '/' || c == '\\') {
            indent++;
        } else {
            break;
        }
    }
    return indent / 4; // Convert to indent level
}

bool MarkdownParser::is_tree_line(const std::string& line) {
    // Check if line contains tree-like characters or proper indentation
    if (line.empty()) return false;

    // Look for common tree characters
    return line.find("├") != std::string::npos ||
           line.find("└") != std::string::npos ||
           line.find("│") != std::string::npos ||
           line.find("─") != std::string::npos ||
           line.find("/") != std::string::npos ||
           (line.find_first_not_of(" \t") != std::string::npos &&
            (line.find(".") != std::string::npos || line.find("/") != std::string::npos));
}

std::string MarkdownParser::extract_name(const std::string& line, bool& is_directory) {
    // Remove tree characters and extract name
    std::string cleaned = line;

    // Remove common tree drawing characters
    const std::vector<std::string> tree_chars = {
        "├──", "└──", "│", "├─", "└─", "├", "└", "─", "│  ", "|"
    };

    for (const auto& tc : tree_chars) {
        size_t pos;
        while ((pos = cleaned.find(tc)) != std::string::npos) {
            cleaned.replace(pos, tc.length(), " ");
        }
    }

    // Trim whitespace
    auto start = cleaned.find_first_not_of(" \t");
    auto end = cleaned.find_last_not_of(" \t\r\n");

    if (start == std::string::npos) {
        return "";
    }

    std::string name = cleaned.substr(start, end - start + 1);

    // Check if it's a directory (ends with / or has trailing /)
    if (!name.empty() && name.back() == '/') {
        is_directory = true;
        name.pop_back();
    } else {
        // Check if it looks like a file (has extension)
        is_directory = (name.find('.') == std::string::npos);
    }

    return name;
}

// TreeVisualizer implementation
std::string TreeVisualizer::visualize(const Node& root, bool use_unicode) {
    std::string output;
    output += root.name;
    if (root.is_directory()) {
        output += "/";
    }
    output += "\n";

    for (size_t i = 0; i < root.children.size(); ++i) {
        bool is_last = (i == root.children.size() - 1);
        visualize_recursive(*root.children[i], output, "", is_last, use_unicode);
    }

    return output;
}

void TreeVisualizer::print(const Node& root, bool use_unicode) {
    std::cout << visualize(root, use_unicode);
}

void TreeVisualizer::visualize_recursive(
    const Node& node,
    std::string& output,
    const std::string& prefix,
    bool is_last,
    bool use_unicode
) {
    std::string branch = use_unicode ? (is_last ? "└── " : "├── ") : (is_last ? "`-- " : "|-- ");
    std::string extension = use_unicode ? (is_last ? "    " : "│   ") : (is_last ? "    " : "|   ");

    output += prefix + branch + node.name;
    if (node.is_directory()) {
        output += "/";
    }
    output += "\n";

    std::string child_prefix = prefix + extension;

    for (size_t i = 0; i < node.children.size(); ++i) {
        bool child_is_last = (i == node.children.size() - 1);
        visualize_recursive(*node.children[i], output, child_prefix, child_is_last, use_unicode);
    }
}

} // namespace yaqeen::core
