#include "yaqeen/ui/theme.hpp"
#include "ftxui/dom/elements.hpp"

namespace yaqeen::ui {

using namespace ftxui;

// Tokyo Night Color Palette definitions
const Color TokyoColors::BG = Color::RGB(26, 27, 38);
const Color TokyoColors::FG = Color::RGB(169, 177, 214);
const Color TokyoColors::CYAN = Color::RGB(125, 207, 255);
const Color TokyoColors::GREEN = Color::RGB(158, 206, 106);
const Color TokyoColors::RED = Color::RGB(247, 118, 142);
const Color TokyoColors::YELLOW = Color::RGB(224, 175, 104);
const Color TokyoColors::MAGENTA = Color::RGB(187, 154, 247);
const Color TokyoColors::ORANGE = Color::RGB(255, 158, 100);
const Color TokyoColors::BLUE = Color::RGB(122, 162, 247);
const Color TokyoColors::COMMENT = Color::RGB(86, 95, 137);
const Color TokyoColors::BG_HIGHLIGHT = Color::RGB(41, 46, 66);
const Color TokyoColors::BG_DARK = Color::RGB(22, 22, 30);

Element Theme::colored_text(const std::string& text, const Color& color) {
    return ftxui::text(text) | ftxui::color(color);
}

Element Theme::success_text(const std::string& text) {
    return colored_text(text, TokyoColors::GREEN);
}

Element Theme::error_text(const std::string& text) {
    return colored_text(text, TokyoColors::RED);
}

Element Theme::info_text(const std::string& text) {
    return colored_text(text, TokyoColors::CYAN);
}

Element Theme::warn_text(const std::string& text) {
    return colored_text(text, TokyoColors::YELLOW);
}

Element Theme::highlight_text(const std::string& text) {
    return colored_text(text, TokyoColors::MAGENTA);
}

Element Theme::dim_text(const std::string& text) {
    return colored_text(text, TokyoColors::COMMENT);
}

Element Theme::styled_box(Element content) {
    return window(text(""), std::move(content))
        | color(TokyoColors::CYAN)
        | bgcolor(TokyoColors::BG_DARK);
}

Element Theme::title_box(const std::string& title, Element content) {
    return window(text(title) | bold | color(TokyoColors::MAGENTA), std::move(content))
        | color(TokyoColors::CYAN)
        | bgcolor(TokyoColors::BG_DARK);
}

Element Theme::success_indicator() {
    return text("[+]") | bold | color(TokyoColors::GREEN);
}

Element Theme::error_indicator() {
    return text("[!]") | bold | color(TokyoColors::RED);
}

Element Theme::info_indicator() {
    return text("[*]") | bold | color(TokyoColors::CYAN);
}

Element Theme::spinner_indicator(int frame) {
    const std::vector<std::string> chars = {"|", "/", "-", "\\"};
    return text(chars[frame % chars.size()]) | bold | color(TokyoColors::MAGENTA);
}

Element Theme::horizontal_line() {
    return separator() | color(TokyoColors::COMMENT);
}

Element Theme::vertical_separator() {
    return separatorEmpty();
}

// Logo Art
std::vector<std::string> LogoArt::get_logo() {
    return {
        "  ██    ██  █████   ██████  ███████ ███████ ███    ██ ",
        "   ██  ██  ██   ██ ██    ██ ██      ██      ████   ██ ",
        "    ████   ███████ ██    ██ █████   █████   ██ ██  ██ ",
        "     ██    ██   ██ ██  ▄▄██ ██      ██      ██  ██ ██ ",
        "     ██    ██   ██  ██████  ███████ ███████ ██   ████ "
    };
}

Element LogoArt::render_logo(const Color& color) {
    auto logo_lines = get_logo();
    Elements elements;

    for (const auto& line : logo_lines) {
        elements.push_back(text(line) | bold | ftxui::color(color) | center);
    }

    return vbox(std::move(elements));
}

Element LogoArt::render_animated_logo(int frame) {
    auto logo_lines = get_logo();
    Elements elements;

    // Cycle through colors for wave effect
    const std::vector<Color> colors = {
        TokyoColors::CYAN,
        TokyoColors::BLUE,
        TokyoColors::MAGENTA,
        TokyoColors::MAGENTA,
        TokyoColors::BLUE
    };

    for (size_t i = 0; i < logo_lines.size(); ++i) {
        int color_index = (i + (frame / 5)) % colors.size();
        elements.push_back(
            text(logo_lines[i]) | bold | color(colors[color_index]) | center
        );
    }

    return vbox(std::move(elements));
}

} // namespace yaqeen::ui
