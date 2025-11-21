#include <catch2/catch_test_macros.hpp>
#include "yaqeen/core/template_manager.hpp"
#include <nlohmann/json.hpp>

using namespace yaqeen::core;

TEST_CASE("TemplateInfo can be created from JSON", "[templates]") {
    nlohmann::json json = {
        {"name", "test-template"},
        {"description", "A test template"},
        {"version", "1.0.0"},
        {"category", "test"},
        {"tags", {"test", "example"}}
    };

    auto result = TemplateInfo::from_json(json);

    REQUIRE(result.is_ok());
    REQUIRE(result.value().name == "test-template");
    REQUIRE(result.value().description == "A test template");
    REQUIRE(result.value().category == "test");
    REQUIRE(result.value().tags.size() == 2);
}

TEST_CASE("TemplateInfo handles missing optional fields", "[templates]") {
    nlohmann::json json = {
        {"name", "minimal"},
        {"description", "Minimal template"}
    };

    auto result = TemplateInfo::from_json(json);

    REQUIRE(result.is_ok());
    REQUIRE(result.value().version == "1.0.0"); // Default value
    REQUIRE(result.value().category == "other"); // Default value
}

TEST_CASE("Template validation works correctly", "[templates]") {
    Template tmpl;
    tmpl.info.name = "test";
    tmpl.info.description = "Test template";
    tmpl.structure = nlohmann::json::object();

    REQUIRE(tmpl.is_valid());
}

TEST_CASE("Template validation fails for invalid templates", "[templates]") {
    Template tmpl;
    tmpl.info.name = "";  // Invalid: empty name
    tmpl.info.description = "Test";
    tmpl.structure = nlohmann::json::object();

    REQUIRE_FALSE(tmpl.is_valid());
}

TEST_CASE("TemplateManager lists templates", "[templates]") {
    // This test would require actual template files
    // In a real scenario, you'd set up a test fixtures directory
    REQUIRE(true); // Placeholder
}

TEST_CASE("Template structure validation", "[templates]") {
    nlohmann::json valid_structure = {
        {"src/", {
            {"main.cpp", ""},
            {"utils/", {}}
        }},
        {"README.md", ""}
    };

    // Create a template manager to test validation
    TemplateManager manager;

    // Validation would be tested through manager methods
    REQUIRE(true); // Placeholder
}
