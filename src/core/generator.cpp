#include "yaqeen/core/generator.hpp"
#include "yaqeen/utils/logger.hpp"
#include "yaqeen/utils/validators.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>

namespace yaqeen::core {

// GenerationStats implementation
std::string GenerationStats::to_string() const {
    std::ostringstream oss;
    oss << "Statistics:\n";
    oss << "  Files created: " << files_created << "\n";
    oss << "  Directories created: " << dirs_created << "\n";
    oss << "  Total size: " << total_size << " bytes\n";
    oss << "  Time elapsed: " << elapsed.count() << "ms";
    return oss.str();
}

// FileGenerator implementation
FileGenerator::FileGenerator(Options opts) : options_(std::move(opts)) {
}

Result<GenerationStats> FileGenerator::generate(
    const Node& root,
    const std::filesystem::path& output_dir
) {
    //LOG_INFO("Starting generation at: {}", output_dir.string());

    // Validate before generating
    auto validation = validate(root, output_dir);
    if (validation.is_error()) {
        return validation.error();
    }

    // Reset statistics
    stats_ = GenerationStats{};
    start_time_ = std::chrono::steady_clock::now();

    // Count total nodes for progress tracking
    size_t total = count_nodes(root);
    size_t current = 0;

    // Generate from root
    auto result = generate_node(root, output_dir, current, total);
    if (result.is_error()) {
        return result.error();
    }

    // Calculate elapsed time
    auto end_time = std::chrono::steady_clock::now();
    stats_.elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time_
    );

    //LOG_INFO("Generation complete: {} files, {} directories",
    //         stats_.files_created, stats_.dirs_created);

    return stats_;
}

Result<void> FileGenerator::create_file(
    const std::filesystem::path& path,
    const std::string& content
) {
    if (options_.dry_run) {
        //LOG_INFO("[DRY RUN] Would create file: {}", path.string());
        return Result<void>();
    }

    // Check if file already exists
    if (std::filesystem::exists(path) && !options_.overwrite) {
        if (should_skip_existing(path)) {
            //LOG_DEBUG("Skipping existing file: {}", path.string());
            return Result<void>();
        }
        return Error(ErrorCode::FileAlreadyExists, "File already exists: " + path.string());
    }

    // Create parent directory if needed
    auto parent = path.parent_path();
    if (!parent.empty() && !std::filesystem::exists(parent)) {
        std::error_code ec;
        std::filesystem::create_directories(parent, ec);
        if (ec) {
            return Error(ErrorCode::CannotCreateDirectory,
                        "Cannot create parent directory: " + parent.string(),
                        ec.message());
        }
    }

    // Write file
    std::ofstream file(path);
    if (!file) {
        return Error(ErrorCode::CannotCreateFile, "Cannot create file: " + path.string());
    }

    if (!content.empty()) {
        file << content;
    }

    file.close();

    //LOG_DEBUG("Created file: {}", path.string());
    return Result<void>();
}

Result<void> FileGenerator::create_directory(const std::filesystem::path& path) {
    if (options_.dry_run) {
        //LOG_INFO("[DRY RUN] Would create directory: {}", path.string());
        return Result<void>();
    }

    if (std::filesystem::exists(path)) {
        if (std::filesystem::is_directory(path)) {
            //LOG_DEBUG("Directory already exists: {}", path.string());
            return Result<void>();
        }
        return Error(ErrorCode::FileAlreadyExists,
                    "Path exists but is not a directory: " + path.string());
    }

    std::error_code ec;
    std::filesystem::create_directories(path, ec);
    if (ec) {
        return Error(ErrorCode::CannotCreateDirectory,
                    "Cannot create directory: " + path.string(),
                    ec.message());
    }

    //LOG_DEBUG("Created directory: {}", path.string());
    return Result<void>();
}

Result<void> FileGenerator::validate(const Node& root, const std::filesystem::path& output_dir) {
    // Check if output directory's parent exists
    if (!output_dir.empty()) {
        auto parent = output_dir.parent_path();
        if (!parent.empty() && !std::filesystem::exists(parent)) {
            return Error(ErrorCode::DirectoryNotFound,
                        "Parent directory does not exist: " + parent.string());
        }
    }

    // Check write permissions
    auto writable = Validator::validate_path_writable(output_dir);
    if (writable.is_error()) {
        return writable;
    }

    return Result<void>();
}

Result<void> FileGenerator::generate_node(
    const Node& node,
    const std::filesystem::path& current_path,
    size_t& current,
    size_t total
) {
    current++;

    notify_progress(current_path, node.is_directory(), current, total);

    if (node.is_directory()) {
        // Create directory
        auto result = create_directory(current_path);
        if (result.is_error()) {
            return result;
        }

        stats_.dirs_created++;

        // Process children
        for (const auto& child : node.children) {
            auto child_path = current_path / child->name;
            auto child_result = generate_node(*child, child_path, current, total);
            if (child_result.is_error()) {
                return child_result;
            }
        }
    } else {
        // Create file
        std::string content = node.content.value_or("");
        auto result = create_file(current_path, content);
        if (result.is_error()) {
            return result;
        }

        stats_.files_created++;

        // Update total size
        if (std::filesystem::exists(current_path)) {
            stats_.total_size += std::filesystem::file_size(current_path);
        }
    }

    return Result<void>();
}

size_t FileGenerator::count_nodes(const Node& node) const {
    size_t count = 1; // Count this node

    for (const auto& child : node.children) {
        count += count_nodes(*child);
    }

    return count;
}

bool FileGenerator::should_skip_existing(const std::filesystem::path& path) const {
    // In non-overwrite mode, skip existing files
    return !options_.overwrite;
}

void FileGenerator::notify_progress(
    const std::filesystem::path& path,
    bool is_directory,
    size_t current,
    size_t total
) {
    if (options_.progress_callback) {
        options_.progress_callback(path, is_directory, current, total);
    }

    if (options_.verbose) {
        std::string type = is_directory ? "DIR " : "FILE";
        //LOG_INFO("[{}/{}] {} {}", current, total, type, path.string());
        (void)type;  // Unused
    }
}

// TemplateGenerator implementation
Result<GenerationStats> TemplateGenerator::generate_from_json(
    const nlohmann::json& structure,
    const TemplateOptions& options
) {
    //LOG_INFO("Generating from template for project: {}", options.project_name);

    // Convert JSON to node tree
    auto tree_result = json_to_node_tree(structure, options.project_name);
    if (tree_result.is_error()) {
        return tree_result.error();
    }

    auto root = std::move(tree_result.value());

    // Create file generator
    FileGenerator::Options gen_options;
    gen_options.dry_run = options.dry_run;
    gen_options.verbose = options.verbose;
    gen_options.progress_callback = options.progress_callback;

    FileGenerator generator(gen_options);

    return generator.generate(*root, options.output_dir);
}

Result<std::unique_ptr<Node>> TemplateGenerator::json_to_node_tree(
    const nlohmann::json& json_obj,
    const std::string& root_name
) {
    if (!json_obj.is_object()) {
        return Error(ErrorCode::InvalidJSONFormat,
                    "Template structure must be a JSON object");
    }

    auto root = std::make_unique<Node>(Node::Type::Directory, root_name);
    json_to_node_recursive(json_obj, *root);

    return root;
}

void TemplateGenerator::json_to_node_recursive(
    const nlohmann::json& json_obj,
    Node& parent_node
) {
    for (auto it = json_obj.begin(); it != json_obj.end(); ++it) {
        std::string name = it.key();
        const auto& value = it.value();

        // Determine if this is a file or directory
        bool is_directory = name.back() == '/' || value.is_object();

        // Remove trailing slash if present
        if (!name.empty() && name.back() == '/') {
            name.pop_back();
        }

        if (is_directory) {
            // Create directory node
            auto dir_node = std::make_unique<Node>(Node::Type::Directory, name);

            // Recursively process children if it's an object
            if (value.is_object()) {
                json_to_node_recursive(value, *dir_node);
            }

            parent_node.add_child(std::move(dir_node));
        } else {
            // Create file node
            auto file_node = std::make_unique<Node>(Node::Type::File, name);

            // Set content if provided as string
            if (value.is_string() && !value.get<std::string>().empty()) {
                file_node->content = value.get<std::string>();
            }

            parent_node.add_child(std::move(file_node));
        }
    }
}

} // namespace yaqeen::core
