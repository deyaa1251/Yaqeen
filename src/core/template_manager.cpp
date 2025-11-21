#include "yaqeen/core/template_manager.hpp"
#include "yaqeen/utils/logger.hpp"
#include "yaqeen/utils/validators.hpp"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <set>

namespace yaqeen::core {

// TemplateInfo implementation
Result<TemplateInfo> TemplateInfo::from_json(const nlohmann::json& json) {
    try {
        TemplateInfo info;
        info.name = json.at("name").get<std::string>();
        info.description = json.at("description").get<std::string>();
        info.version = json.value("version", "1.0.0");
        info.category = json.value("category", "other");

        if (json.contains("tags") && json["tags"].is_array()) {
            info.tags = json["tags"].get<std::vector<std::string>>();
        }

        if (json.contains("author")) {
            info.author = json["author"].get<std::string>();
        }

        if (json.contains("repository")) {
            info.repository = json["repository"].get<std::string>();
        }

        return info;
    } catch (const nlohmann::json::exception& e) {
        return Error(ErrorCode::InvalidJSONFormat,
                    "Failed to parse template info",
                    e.what());
    }
}

nlohmann::json TemplateInfo::to_json() const {
    nlohmann::json j;
    j["name"] = name;
    j["description"] = description;
    j["version"] = version;
    j["category"] = category;
    j["tags"] = tags;

    if (author.has_value()) {
        j["author"] = *author;
    }

    if (repository.has_value()) {
        j["repository"] = *repository;
    }

    return j;
}

// Template implementation
Result<Template> Template::load_from_file(const std::filesystem::path& path) {
    // Validate file
    auto validation = Validator::validate_file_readable(path);
    if (validation.is_error()) {
        return validation.error();
    }

    // Read file
    std::ifstream file(path);
    if (!file) {
        return Error(ErrorCode::FileNotFound, "Cannot open template file: " + path.string());
    }

    // Parse JSON
    nlohmann::json json;
    try {
        file >> json;
    } catch (const nlohmann::json::parse_error& e) {
        return Error(ErrorCode::InvalidJSONFormat,
                    "Failed to parse template JSON: " + path.string(),
                    e.what());
    }

    // Extract template info
    auto info_result = TemplateInfo::from_json(json);
    if (info_result.is_error()) {
        return info_result.error();
    }

    // Extract structure
    if (!json.contains("structure")) {
        return Error(ErrorCode::InvalidTemplateStructure,
                    "Template missing 'structure' field: " + path.string());
    }

    Template tmpl;
    tmpl.info = std::move(info_result.value());
    tmpl.structure = json["structure"];
    tmpl.source_path = path;

    return tmpl;
}

bool Template::is_valid() const {
    return !info.name.empty() &&
           !info.description.empty() &&
           structure.is_object();
}

// TemplateManager implementation
TemplateManager::TemplateManager()
    : templates_dir_(get_default_templates_directory())
    , initialized_(false) {
}

TemplateManager::TemplateManager(const std::filesystem::path& templates_dir)
    : templates_dir_(templates_dir)
    , initialized_(false) {
}

Result<void> TemplateManager::initialize() {
    LOG_INFO("Initializing template manager");

    // Check if templates directory exists
    if (!std::filesystem::exists(templates_dir_)) {
        return Error(ErrorCode::DirectoryNotFound,
                    "Templates directory not found: " + templates_dir_.string());
    }

    // Load all templates
    return load_templates();
}

Result<void> TemplateManager::load_templates() {
    LOG_INFO("Loading templates from: " + templates_dir_.string());

    templates_.clear();

    // Scan directory recursively
    scan_directory(templates_dir_);

    LOG_INFO("Loaded " + std::to_string(templates_.size()) + " templates");
    initialized_ = true;

    return Result<void>();
}

std::vector<std::string> TemplateManager::list_templates() const {
    std::vector<std::string> names;
    names.reserve(templates_.size());

    for (const auto& [name, _] : templates_) {
        names.push_back(name);
    }

    std::sort(names.begin(), names.end());
    return names;
}

std::vector<std::string> TemplateManager::list_categories() const {
    std::set<std::string> categories_set;

    for (const auto& [_, tmpl] : templates_) {
        categories_set.insert(tmpl.info.category);
    }

    std::vector<std::string> categories(categories_set.begin(), categories_set.end());
    std::sort(categories.begin(), categories.end());
    return categories;
}

std::vector<TemplateInfo> TemplateManager::list_templates_in_category(
    const std::string& category
) const {
    std::vector<TemplateInfo> templates;

    for (const auto& [_, tmpl] : templates_) {
        if (tmpl.info.category == category) {
            templates.push_back(tmpl.info);
        }
    }

    return templates;
}

std::vector<TemplateInfo> TemplateManager::search_templates(const std::string& query) const {
    std::vector<TemplateInfo> results;
    std::string lower_query = query;
    std::transform(lower_query.begin(), lower_query.end(), lower_query.begin(), ::tolower);

    for (const auto& [_, tmpl] : templates_) {
        std::string lower_name = tmpl.info.name;
        std::string lower_desc = tmpl.info.description;
        std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
        std::transform(lower_desc.begin(), lower_desc.end(), lower_desc.begin(), ::tolower);

        if (lower_name.find(lower_query) != std::string::npos ||
            lower_desc.find(lower_query) != std::string::npos) {
            results.push_back(tmpl.info);
        }
    }

    return results;
}

Result<Template> TemplateManager::get_template(const std::string& name) const {
    auto it = templates_.find(name);
    if (it == templates_.end()) {
        return Error(ErrorCode::TemplateNotFound, "Template not found: " + name);
    }

    return it->second;
}

Result<TemplateInfo> TemplateManager::get_template_info(const std::string& name) const {
    auto result = get_template(name);
    if (result.is_error()) {
        return result.error();
    }

    return result.value().info;
}

bool TemplateManager::has_template(const std::string& name) const {
    return templates_.find(name) != templates_.end();
}

Result<GenerationStats> TemplateManager::generate_from_template(
    const std::string& template_name,
    const std::filesystem::path& output_dir,
    const std::string& project_name,
    const TemplateGenerator::TemplateOptions& options
) {
    LOG_INFO("Generating project from template: " + template_name);

    // Get template
    auto tmpl_result = get_template(template_name);
    if (tmpl_result.is_error()) {
        return tmpl_result.error();
    }

    const auto& tmpl = tmpl_result.value();

    // Validate template
    auto validation = validate_template(tmpl);
    if (validation.is_error()) {
        return validation.error();
    }

    // Generate using TemplateGenerator
    TemplateGenerator generator;
    return generator.generate_from_json(tmpl.structure, options);
}

Result<void> TemplateManager::validate_template(const Template& tmpl) const {
    if (!tmpl.is_valid()) {
        return Error(ErrorCode::TemplateInvalid, "Template is invalid: " + tmpl.info.name);
    }

    if (!validate_structure_recursive(tmpl.structure)) {
        return Error(ErrorCode::InvalidTemplateStructure,
                    "Template structure is invalid: " + tmpl.info.name);
    }

    return Result<void>();
}

Result<void> TemplateManager::validate_all_templates() {
    for (const auto& [name, tmpl] : templates_) {
        auto result = validate_template(tmpl);
        if (result.is_error()) {
            LOG_ERROR("Template validation failed: " + name);
            return result;
        }
    }

    LOG_INFO("All templates validated successfully");
    return Result<void>();
}

std::filesystem::path TemplateManager::get_default_templates_directory() {
    // Try multiple locations
    std::vector<std::filesystem::path> candidates = {
        "./templates",
        "../templates",
        "/usr/share/yaqeen/templates",
        "/usr/local/share/yaqeen/templates"
    };

    for (const auto& path : candidates) {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            return path;
        }
    }

    // Default to ./templates
    return "./templates";
}

Result<Template> TemplateManager::load_template_file(const std::filesystem::path& file_path) {
    LOG_DEBUG("Loading template: " + file_path.string());

    auto result = Template::load_from_file(file_path);
    if (result.is_error()) {
        LOG_ERROR("Failed to load template: " + file_path.string());
        return result;
    }

    return result;
}

void TemplateManager::scan_directory(const std::filesystem::path& dir) {
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                auto result = load_template_file(entry.path());
                if (result.is_ok()) {
                    const auto& tmpl = result.value();
                    templates_[tmpl.info.name] = std::move(result.value());
                    LOG_DEBUG("Loaded template: " + tmpl.info.name);
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        LOG_ERROR(std::string("Error scanning directory: ") + e.what());
    }
}

bool TemplateManager::validate_structure_recursive(const nlohmann::json& structure) const {
    if (!structure.is_object()) {
        return false;
    }

    for (auto it = structure.begin(); it != structure.end(); ++it) {
        const auto& value = it.value();

        // Value can be empty string (file), string (file with content), or object (directory)
        if (value.is_object()) {
            if (!validate_structure_recursive(value)) {
                return false;
            }
        } else if (!value.is_string()) {
            return false;
        }
    }

    return true;
}

// TemplateDisplay implementation
std::string TemplateDisplay::format_template_list(const std::vector<TemplateInfo>& templates) {
    std::ostringstream oss;

    for (const auto& tmpl : templates) {
        oss << "  " << std::setw(30) << std::left << tmpl.name
            << " - " << tmpl.description << "\n";
    }

    return oss.str();
}

std::string TemplateDisplay::format_template_details(const Template& tmpl) {
    std::ostringstream oss;

    oss << "Template: " << tmpl.info.name << "\n";
    oss << "Description: " << tmpl.info.description << "\n";
    oss << "Version: " << tmpl.info.version << "\n";
    oss << "Category: " << tmpl.info.category << "\n";

    if (!tmpl.info.tags.empty()) {
        oss << "Tags: ";
        for (size_t i = 0; i < tmpl.info.tags.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << tmpl.info.tags[i];
        }
        oss << "\n";
    }

    if (tmpl.info.author.has_value()) {
        oss << "Author: " << *tmpl.info.author << "\n";
    }

    if (tmpl.info.repository.has_value()) {
        oss << "Repository: " << *tmpl.info.repository << "\n";
    }

    return oss.str();
}

std::string TemplateDisplay::format_categories(const std::vector<std::string>& categories) {
    std::ostringstream oss;

    oss << "Available categories:\n";
    for (const auto& category : categories) {
        oss << "  - " << category << "\n";
    }

    return oss.str();
}

void TemplateDisplay::print_template_list(const std::vector<TemplateInfo>& templates) {
    std::cout << format_template_list(templates);
}

void TemplateDisplay::print_template_details(const Template& tmpl) {
    std::cout << format_template_details(tmpl);
}

void TemplateDisplay::print_categories(const std::vector<std::string>& categories) {
    std::cout << format_categories(categories);
}

} // namespace yaqeen::core
