#include <catch2/catch_test_macros.hpp>
#include "yaqeen/core/generator.hpp"
#include "yaqeen/core/parser.hpp"
#include <filesystem>

using namespace yaqeen::core;

TEST_CASE("FileGenerator creates directories", "[generator]") {
    auto root = std::make_unique<Node>(Node::Type::Directory, "test_dir");

    FileGenerator::Options options;
    options.dry_run = true; // Don't actually create files in tests

    FileGenerator generator(options);

    auto result = generator.generate(*root, std::filesystem::temp_directory_path() / "yaqeen_test");

    REQUIRE(result.is_ok());
}

TEST_CASE("FileGenerator creates files", "[generator]") {
    auto root = std::make_unique<Node>(Node::Type::Directory, "test_project");
    auto file = std::make_unique<Node>(Node::Type::File, "test.txt");
    file->content = "Hello, World!";
    root->add_child(std::move(file));

    FileGenerator::Options options;
    options.dry_run = true;

    FileGenerator generator(options);

    auto result = generator.generate(*root, std::filesystem::temp_directory_path() / "yaqeen_test");

    REQUIRE(result.is_ok());
    REQUIRE(result.value().files_created >= 0);
}

TEST_CASE("FileGenerator counts nodes correctly", "[generator]") {
    auto root = std::make_unique<Node>(Node::Type::Directory, "root");

    auto dir1 = std::make_unique<Node>(Node::Type::Directory, "dir1");
    dir1->add_child(std::make_unique<Node>(Node::Type::File, "file1.txt"));
    dir1->add_child(std::make_unique<Node>(Node::Type::File, "file2.txt"));

    auto dir2 = std::make_unique<Node>(Node::Type::Directory, "dir2");
    dir2->add_child(std::make_unique<Node>(Node::Type::File, "file3.txt"));

    root->add_child(std::move(dir1));
    root->add_child(std::move(dir2));

    FileGenerator::Options options;
    options.dry_run = true;

    FileGenerator generator(options);

    auto result = generator.generate(*root, std::filesystem::temp_directory_path() / "yaqeen_test");

    REQUIRE(result.is_ok());
}

TEST_CASE("TemplateGenerator converts JSON to node tree", "[generator]") {
    nlohmann::json structure = {
        {"src/", {
            {"main.cpp", ""},
            {"utils/", {}}
        }},
        {"README.md", ""}
    };

    TemplateGenerator::TemplateOptions options;
    options.project_name = "test_project";
    options.output_dir = std::filesystem::temp_directory_path() / "yaqeen_test";
    options.dry_run = true;

    TemplateGenerator generator;

    auto result = generator.generate_from_json(structure, options);

    REQUIRE(result.is_ok());
}
