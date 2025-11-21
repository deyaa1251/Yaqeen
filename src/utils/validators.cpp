#include "yaqeen/utils/validators.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

namespace yaqeen {

const std::regex Validator::PROJECT_NAME_PATTERN(R"(^[a-zA-Z0-9_-]+$)");
const std::regex Validator::TEMPLATE_NAME_PATTERN(R"(^[a-zA-Z0-9_-]+$)");

Result<void> Validator::validate_file_exists(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        return Error(ErrorCode::FileNotFound, "File not found: " + path.string());
    }
    if (!std::filesystem::is_regular_file(path)) {
        return Error(ErrorCode::InvalidInput, "Path is not a file: " + path.string());
    }
    return Result<void>();
}

Result<void> Validator::validate_directory_exists(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        return Error(ErrorCode::DirectoryNotFound, "Directory not found: " + path.string());
    }
    if (!std::filesystem::is_directory(path)) {
        return Error(ErrorCode::InvalidInput, "Path is not a directory: " + path.string());
    }
    return Result<void>();
}

Result<void> Validator::validate_path_writable(const std::filesystem::path& path) {
    auto parent = path.parent_path();
    if (parent.empty()) {
        parent = std::filesystem::current_path();
    }

    if (!std::filesystem::exists(parent)) {
        return Error(ErrorCode::DirectoryNotFound, "Parent directory does not exist: " + parent.string());
    }

    // Check write permissions by attempting to create a test file
    auto test_path = parent / (".yaqeen_write_test_" + std::to_string(std::time(nullptr)));
    std::ofstream test_file(test_path);
    if (!test_file) {
        return Error(ErrorCode::PermissionDenied, "Cannot write to directory: " + parent.string());
    }
    test_file.close();
    std::filesystem::remove(test_path);

    return Result<void>();
}

Result<void> Validator::validate_file_readable(const std::filesystem::path& path) {
    auto result = validate_file_exists(path);
    if (result.is_error()) {
        return result;
    }

    std::ifstream file(path);
    if (!file) {
        return Error(ErrorCode::PermissionDenied, "Cannot read file: " + path.string());
    }

    return Result<void>();
}

Result<void> Validator::validate_project_name(std::string_view name) {
    if (name.empty()) {
        return Error(ErrorCode::InvalidInput, "Project name cannot be empty");
    }

    if (name.length() > 255) {
        return Error(ErrorCode::InvalidInput, "Project name too long (max 255 characters)");
    }

    if (!std::regex_match(name.begin(), name.end(), PROJECT_NAME_PATTERN)) {
        return Error(
            ErrorCode::InvalidInput,
            "Invalid project name. Use only letters, numbers, hyphens, and underscores"
        );
    }

    return Result<void>();
}

Result<void> Validator::validate_template_name(std::string_view name) {
    if (name.empty()) {
        return Error(ErrorCode::InvalidInput, "Template name cannot be empty");
    }

    if (!std::regex_match(name.begin(), name.end(), TEMPLATE_NAME_PATTERN)) {
        return Error(
            ErrorCode::InvalidInput,
            "Invalid template name. Use only letters, numbers, hyphens, and underscores"
        );
    }

    return Result<void>();
}

Result<void> Validator::validate_json_format(std::string_view json_content) {
    try {
        auto parsed = nlohmann::json::parse(json_content);
        return Result<void>();
    } catch (const nlohmann::json::parse_error& e) {
        return Error(ErrorCode::InvalidJSONFormat, "Invalid JSON format", e.what());
    }
}

bool Validator::is_valid_directory_name(std::string_view name) {
    if (name.empty() || name == "." || name == "..") {
        return false;
    }

    // Check for invalid characters
    const std::string invalid_chars = "<>:\"|?*\0";
    for (char c : name) {
        if (invalid_chars.find(c) != std::string::npos) {
            return false;
        }
        if (c < 32) { // Control characters
            return false;
        }
    }

    return true;
}

bool Validator::is_valid_filename(std::string_view name) {
    return is_valid_directory_name(name);
}

bool Validator::has_markdown_extension(const std::filesystem::path& path) {
    auto ext = path.extension().string();
    return ext == ".md" || ext == ".markdown";
}

bool Validator::has_json_extension(const std::filesystem::path& path) {
    return path.extension() == ".json";
}

} // namespace yaqeen
