#include "yaqeen/core/parser.hpp"
#include "yaqeen/core/generator.hpp"
#include "yaqeen/core/template_manager.hpp"
#include "yaqeen/ui/theme.hpp"
#include "yaqeen/ui/animations.hpp"
#include "yaqeen/ui/progress.hpp"
#include "yaqeen/utils/logger.hpp"
#include "yaqeen/utils/validators.hpp"

#include <CLI/CLI.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace yaqeen;
using namespace ftxui;

// Global settings
struct GlobalSettings {
    bool verbose = false;
    bool dry_run = false;
    std::string log_file;
    std::string templates_dir;
} g_settings;

void print_logo() {
    auto logo = ui::LogoArt::render_logo(ui::TokyoColors::CYAN);
    auto subtitle = text("Project Structure Generator") | center | color(ui::TokyoColors::COMMENT);

    auto document = vbox({
        text("") | size(HEIGHT, EQUAL, 1),
        logo,
        text("") | size(HEIGHT, EQUAL, 1),
        subtitle,
        text("") | size(HEIGHT, EQUAL, 1)
    });

    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}

void print_success(const std::string& message) {
    auto element = hbox({
        ui::Theme::success_indicator(),
        text(" "),
        ui::Theme::success_text(message) | bold
    });
    auto screen = Screen::Create(Dimension::Fit(element));
    Render(screen, element);
    screen.Print();
    std::cout << std::endl;
}

void print_error(const std::string& message) {
    auto element = hbox({
        ui::Theme::error_indicator(),
        text(" "),
        ui::Theme::error_text(message)
    });
    auto screen = Screen::Create(Dimension::Fit(element));
    Render(screen, element);
    screen.Print();
    std::cout << std::endl;
}

void print_info(const std::string& message) {
    auto element = hbox({
        ui::Theme::info_indicator(),
        text(" "),
        ui::Theme::info_text(message)
    });
    auto screen = Screen::Create(Dimension::Fit(element));
    Render(screen, element);
    screen.Print();
    std::cout << std::endl;
}

int cmd_init(const std::string& markdown_file, const std::string& output_dir) {
    print_logo();

    print_info("Initializing project from markdown file");
    std::cout << std::endl;

    // Parse markdown file
    print_info("Parsing markdown structure...");
    core::MarkdownParser parser;
    auto parse_result = parser.parse(markdown_file);

    if (parse_result.is_error()) {
        print_error("Failed to parse markdown: " + parse_result.error().message);
        return 1;
    }

    auto tree = std::move(parse_result.value());

    // Preview structure
    if (g_settings.verbose) {
        std::cout << std::endl;
        print_info("Project structure:");
        std::cout << core::TreeVisualizer::visualize(*tree, true) << std::endl;
    }

    // Generate files
    print_info("Generating project structure...");

    core::FileGenerator::Options options;
    options.dry_run = g_settings.dry_run;
    options.verbose = g_settings.verbose;

    core::FileGenerator generator(options);
    auto gen_result = generator.generate(*tree, output_dir.empty() ? "." : output_dir);

    if (gen_result.is_error()) {
        print_error("Generation failed: " + gen_result.error().message);
        return 1;
    }

    std::cout << std::endl;
    const auto& stats = gen_result.value();

    // Print summary
    auto summary = vbox({
        ui::Theme::horizontal_line(),
        text("") | size(HEIGHT, EQUAL, 1),
        ui::Theme::success_text("Project created successfully!") | bold | center,
        text("") | size(HEIGHT, EQUAL, 1),
        hbox({
            text("  "),
            vbox({
                hbox({
                    text("Directories: ") | color(ui::TokyoColors::FG),
                    text(std::to_string(stats.dirs_created)) | color(ui::TokyoColors::GREEN) | bold
                }),
                hbox({
                    text("Files: ") | color(ui::TokyoColors::FG),
                    text(std::to_string(stats.files_created)) | color(ui::TokyoColors::GREEN) | bold
                }),
                hbox({
                    text("Time: ") | color(ui::TokyoColors::FG),
                    text(std::to_string(stats.elapsed.count()) + "ms") | color(ui::TokyoColors::YELLOW) | bold
                })
            })
        }),
        text("") | size(HEIGHT, EQUAL, 1),
        ui::Theme::horizontal_line()
    });

    auto screen = Screen::Create(Dimension::Full());
    Render(screen, summary);
    screen.Print();

    return 0;
}

int cmd_create(const std::string& template_name, const std::string& project_name, const std::string& output_dir) {
    print_logo();

    print_info("Creating project: " + project_name);
    print_info("Template: " + template_name);
    std::cout << std::endl;

    // Initialize template manager
    core::TemplateManager manager;
    if (!g_settings.templates_dir.empty()) {
        manager = core::TemplateManager(g_settings.templates_dir);
    }

    auto init_result = manager.initialize();
    if (init_result.is_error()) {
        print_error("Failed to initialize templates: " + init_result.error().message);
        return 1;
    }

    // Check if template exists
    if (!manager.has_template(template_name)) {
        print_error("Template not found: " + template_name);
        std::cout << std::endl;
        print_info("Available templates:");
        auto templates = manager.list_templates();
        for (const auto& t : templates) {
            std::cout << "  - " << t << std::endl;
        }
        return 1;
    }

    // Prepare output directory
    std::filesystem::path out_path = output_dir.empty() ? project_name : output_dir;

    print_info("Generating project structure...");

    // Generate from template
    core::TemplateGenerator::TemplateOptions options;
    options.project_name = project_name;
    options.output_dir = out_path;
    options.dry_run = g_settings.dry_run;
    options.verbose = g_settings.verbose;

    auto gen_result = manager.generate_from_template(template_name, out_path, project_name, options);

    if (gen_result.is_error()) {
        print_error("Generation failed: " + gen_result.error().message);
        return 1;
    }

    std::cout << std::endl;
    const auto& stats = gen_result.value();

    // Print summary
    auto summary = vbox({
        ui::Theme::horizontal_line(),
        text("") | size(HEIGHT, EQUAL, 1),
        ui::Theme::success_text("Project created successfully!") | bold | center,
        text("") | size(HEIGHT, EQUAL, 1),
        hbox({
            text("  "),
            vbox({
                hbox({
                    text("Directories: ") | color(ui::TokyoColors::FG),
                    text(std::to_string(stats.dirs_created)) | color(ui::TokyoColors::GREEN) | bold
                }),
                hbox({
                    text("Files: ") | color(ui::TokyoColors::FG),
                    text(std::to_string(stats.files_created)) | color(ui::TokyoColors::GREEN) | bold
                }),
                hbox({
                    text("Time: ") | color(ui::TokyoColors::FG),
                    text(std::to_string(stats.elapsed.count()) + "ms") | color(ui::TokyoColors::YELLOW) | bold
                })
            })
        }),
        text("") | size(HEIGHT, EQUAL, 1),
        ui::Theme::horizontal_line()
    });

    auto screen = Screen::Create(Dimension::Full());
    Render(screen, summary);
    screen.Print();

    return 0;
}

int cmd_list(const std::string& category) {
    print_logo();

    // Initialize template manager
    core::TemplateManager manager;
    if (!g_settings.templates_dir.empty()) {
        manager = core::TemplateManager(g_settings.templates_dir);
    }

    auto init_result = manager.initialize();
    if (init_result.is_error()) {
        print_error("Failed to initialize templates: " + init_result.error().message);
        return 1;
    }

    if (category.empty()) {
        // List all templates grouped by category
        auto categories = manager.list_categories();

        for (const auto& cat : categories) {
            auto cat_header = text(cat) | bold | color(ui::TokyoColors::MAGENTA);
            auto screen = Screen::Create(Dimension::Fit(cat_header));
            Render(screen, cat_header);
            screen.Print();
            std::cout << std::endl;

            auto templates = manager.list_templates_in_category(cat);
            for (const auto& tmpl : templates) {
                std::cout << "  " << std::setw(25) << std::left << tmpl.name
                         << " - " << tmpl.description << std::endl;
            }
            std::cout << std::endl;
        }
    } else {
        // List templates in specific category
        auto templates = manager.list_templates_in_category(category);
        if (templates.empty()) {
            print_error("No templates found in category: " + category);
            return 1;
        }

        core::TemplateDisplay::print_template_list(templates);
    }

    return 0;
}

int cmd_show(const std::string& template_name) {
    print_logo();

    // Initialize template manager
    core::TemplateManager manager;
    if (!g_settings.templates_dir.empty()) {
        manager = core::TemplateManager(g_settings.templates_dir);
    }

    auto init_result = manager.initialize();
    if (init_result.is_error()) {
        print_error("Failed to initialize templates: " + init_result.error().message);
        return 1;
    }

    auto tmpl_result = manager.get_template(template_name);
    if (tmpl_result.is_error()) {
        print_error("Template not found: " + template_name);
        return 1;
    }

    const auto& tmpl = tmpl_result.value();

    // Display template details with beautiful formatting
    auto details = vbox({
        hbox({
            text("Template: ") | color(ui::TokyoColors::COMMENT),
            text(tmpl.info.name) | color(ui::TokyoColors::CYAN) | bold
        }),
        hbox({
            text("Description: ") | color(ui::TokyoColors::COMMENT),
            text(tmpl.info.description) | color(ui::TokyoColors::FG)
        }),
        hbox({
            text("Category: ") | color(ui::TokyoColors::COMMENT),
            text(tmpl.info.category) | color(ui::TokyoColors::MAGENTA)
        }),
        hbox({
            text("Version: ") | color(ui::TokyoColors::COMMENT),
            text(tmpl.info.version) | color(ui::TokyoColors::YELLOW)
        })
    });

    auto screen = Screen::Create(Dimension::Full());
    Render(screen, details | border | color(ui::TokyoColors::CYAN));
    screen.Print();

    return 0;
}

int main(int argc, char** argv) {
    CLI::App app{"Yaqeen - Project Structure Generator", "yaqeen"};

    app.add_flag("-v,--verbose", g_settings.verbose, "Verbose output");
    app.add_flag("--dry-run", g_settings.dry_run, "Show what would be created without creating");
    app.add_option("--log-file", g_settings.log_file, "Log file path");
    app.add_option("--templates-dir", g_settings.templates_dir, "Custom templates directory");

    // Init command
    auto init_cmd = app.add_subcommand("init", "Initialize from markdown file");
    std::string markdown_file;
    std::string init_output;
    init_cmd->add_option("file", markdown_file, "Markdown file containing project structure")
        ->required();
    init_cmd->add_option("-o,--output", init_output, "Output directory");

    // Create command
    auto create_cmd = app.add_subcommand("create", "Create from template");
    std::string template_name;
    std::string project_name;
    std::string create_output;
    create_cmd->add_option("-t,--template", template_name, "Template name")
        ->required();
    create_cmd->add_option("-n,--name", project_name, "Project name")
        ->required();
    create_cmd->add_option("-o,--output", create_output, "Output directory");

    // List command
    auto list_cmd = app.add_subcommand("list", "List available templates");
    std::string list_category;
    list_cmd->add_option("-c,--category", list_category, "Filter by category");

    // Show command
    auto show_cmd = app.add_subcommand("show", "Show template details");
    std::string show_template;
    show_cmd->add_option("template", show_template, "Template name")
        ->required();

    CLI11_PARSE(app, argc, argv);

    // Setup logger
    if (g_settings.verbose) {
        Logger::instance().set_level(LogLevel::Debug);
    }

    if (!g_settings.log_file.empty()) {
        Logger::instance().set_output_file(g_settings.log_file);
    }

    // Execute commands
    if (init_cmd->parsed()) {
        return cmd_init(markdown_file, init_output);
    } else if (create_cmd->parsed()) {
        return cmd_create(template_name, project_name, create_output);
    } else if (list_cmd->parsed()) {
        return cmd_list(list_category);
    } else if (show_cmd->parsed()) {
        return cmd_show(show_template);
    } else {
        // No command specified, show logo and help
        print_logo();
        std::cout << app.help() << std::endl;
    }

    return 0;
}
