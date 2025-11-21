#include "yaqeen/ui/animations.hpp"
#include "yaqeen/ui/theme.hpp"

namespace yaqeen::ui {

using namespace ftxui;

// Spinner implementation
Spinner::Spinner(Style style) : style_(style) {
    init_frames();
}

void Spinner::init_frames() {
    switch (style_) {
        case Style::Dots:
            frames_ = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
            break;
        case Style::Line:
            frames_ = {"-", "\\", "|", "/"};
            break;
        case Style::Arc:
            frames_ = {"◜", "◠", "◝", "◞", "◡", "◟"};
            break;
        case Style::Circle:
            frames_ = {"◐", "◓", "◑", "◒"};
            break;
        case Style::Bounce:
            frames_ = {"⠁", "⠂", "⠄", "⡀", "⢀", "⠠", "⠐", "⠈"};
            break;
        case Style::BoxBounce:
            frames_ = {"▖", "▘", "▝", "▗"};
            break;
    }
}

Element Spinner::render(int frame) {
    return text(get_char(frame)) | bold | color(TokyoColors::MAGENTA);
}

std::string Spinner::get_char(int frame) const {
    if (frames_.empty()) return "";
    return frames_[frame % frames_.size()];
}

// Animated Logo implementation
AnimatedLogo::AnimatedLogo() {
    init_logo();
}

void AnimatedLogo::init_logo() {
    logo_lines_ = LogoArt::get_logo();
}

Element AnimatedLogo::render(int frame) {
    return LogoArt::render_animated_logo(frame);
}

Element AnimatedLogo::render_with_subtitle(int frame, const std::string& subtitle) {
    return vbox({
        LogoArt::render_animated_logo(frame),
        text("") | size(HEIGHT, EQUAL, 1),
        text(subtitle) | center | color(TokyoColors::COMMENT)
    });
}

Color AnimatedLogo::get_color_for_frame(int frame, int line_index) {
    const std::vector<Color> colors = {
        TokyoColors::CYAN,
        TokyoColors::BLUE,
        TokyoColors::MAGENTA,
        TokyoColors::MAGENTA,
        TokyoColors::BLUE
    };

    int color_index = (line_index + (frame / 5)) % colors.size();
    return colors[color_index];
}

// Typewriter Effect implementation
TypewriterEffect::TypewriterEffect(const std::string& text, int speed_ms)
    : text_(text), speed_ms_(speed_ms) {
}

Element TypewriterEffect::render(int frame) {
    size_t visible_len = get_visible_length(frame);
    std::string visible_text = text_.substr(0, visible_len);
    return text(visible_text) | color(TokyoColors::FG);
}

bool TypewriterEffect::is_complete(int frame) const {
    return get_visible_length(frame) >= text_.length();
}

size_t TypewriterEffect::get_visible_length(int frame) const {
    // Assuming ~60fps, calculate characters to show
    int chars_per_frame = speed_ms_ > 0 ? (1000 / (speed_ms_ * 60)) : 1;
    if (chars_per_frame < 1) chars_per_frame = 1;
    return std::min(static_cast<size_t>(frame / chars_per_frame), text_.length());
}

// Fade-in Effect implementation
FadeInEffect::FadeInEffect(Element element, int duration_frames)
    : element_(std::move(element)), duration_(duration_frames) {
}

Element FadeInEffect::render(int frame) {
    if (frame >= duration_) {
        return element_;
    }
    // Simple fade by gradually showing more opacity
    // FTXUI doesn't have direct opacity control, so we use dim for early frames
    if (frame < duration_ / 3) {
        return element_ | dim;
    }
    return element_;
}

// Loading Animation implementation
LoadingAnimation::LoadingAnimation(const std::string& message)
    : message_(message)
    , running_(false)
    , frame_(0)
    , spinner_(Spinner::Style::Dots) {
}

void LoadingAnimation::start() {
    running_ = true;
    frame_ = 0;
}

void LoadingAnimation::stop() {
    running_ = false;
}

void LoadingAnimation::update_message(const std::string& message) {
    message_ = message;
}

Component LoadingAnimation::component() {
    return CreateComponent();
}

Component LoadingAnimation::CreateComponent() {
    return Renderer([this] {
        if (!running_) {
            return text("");
        }

        frame_++;

        return hbox({
            spinner_.render(frame_),
            text(" ") | size(WIDTH, EQUAL, 1),
            text(message_) | color(TokyoColors::FG)
        });
    });
}

} // namespace yaqeen::ui
