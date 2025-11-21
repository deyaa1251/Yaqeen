#include "yaqeen/ui/progress.hpp"
#include "yaqeen/ui/theme.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace yaqeen::ui {

using namespace ftxui;

// ProgressBar implementation
ProgressBar::ProgressBar(int total, int width)
    : total_(total), width_(width), current_(0) {
}

void ProgressBar::update(int current) {
    current_ = current;
}

void ProgressBar::set_current_item(const std::string& item) {
    current_item_ = item;
}

void ProgressBar::set_status(const std::string& status) {
    status_ = status;
}

Element ProgressBar::render() {
    return vbox({
        render_bar(),
        text("") | size(HEIGHT, EQUAL, 1),
        hbox({
            Theme::info_indicator(),
            text(" "),
            text(current_item_) | color(TokyoColors::FG)
        })
    });
}

Element ProgressBar::render_with_stats() {
    return vbox({
        hbox({
            render_bar(),
            text("  "),
            render_percentage()
        }),
        text("") | size(HEIGHT, EQUAL, 1),
        hbox({
            Theme::info_indicator(),
            text(" "),
            text(current_item_) | color(TokyoColors::COMMENT) | flex
        })
    });
}

Element ProgressBar::render_bar() {
    float percentage = get_percentage();
    int filled = static_cast<int>(width_ * percentage);
    int empty = width_ - filled;

    std::string bar_str;
    bar_str.append(filled, '█');
    bar_str.append(empty, '░');

    return hbox({
        text("[") | color(TokyoColors::COMMENT),
        text(bar_str) | color(TokyoColors::CYAN),
        text("]") | color(TokyoColors::COMMENT)
    });
}

Element ProgressBar::render_percentage() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(0) << (get_percentage() * 100) << "%";
    return text(oss.str()) | color(TokyoColors::YELLOW) | bold;
}

Element ProgressBar::render_fraction() {
    std::ostringstream oss;
    oss << current_.load() << "/" << total_;
    return text(oss.str()) | color(TokyoColors::COMMENT);
}

float ProgressBar::get_percentage() const {
    if (total_ == 0) return 1.0f;
    return static_cast<float>(current_.load()) / static_cast<float>(total_);
}

bool ProgressBar::is_complete() const {
    return current_.load() >= total_;
}

// GenerationStats implementation
GenerationStats::GenerationStats()
    : files_created_(0)
    , dirs_created_(0)
    , timer_running_(false) {
}

void GenerationStats::increment_files() {
    files_created_++;
}

void GenerationStats::increment_directories() {
    dirs_created_++;
}

void GenerationStats::start_timer() {
    start_time_ = std::chrono::steady_clock::now();
    timer_running_ = true;
}

void GenerationStats::stop_timer() {
    end_time_ = std::chrono::steady_clock::now();
    timer_running_ = false;
}

std::chrono::milliseconds GenerationStats::get_elapsed() const {
    if (timer_running_) {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_);
    }
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_time_ - start_time_);
}

Element GenerationStats::render() {
    auto elapsed = get_elapsed();

    return vbox({
        hbox({
            text("Files: ") | color(TokyoColors::COMMENT),
            text(std::to_string(files_created_.load())) | color(TokyoColors::GREEN) | bold
        }),
        hbox({
            text("Directories: ") | color(TokyoColors::COMMENT),
            text(std::to_string(dirs_created_.load())) | color(TokyoColors::BLUE) | bold
        }),
        hbox({
            text("Time: ") | color(TokyoColors::COMMENT),
            text(format_duration(elapsed)) | color(TokyoColors::YELLOW) | bold
        })
    });
}

Element GenerationStats::render_summary() {
    auto elapsed = get_elapsed();

    return vbox({
        Theme::horizontal_line(),
        text("") | size(HEIGHT, EQUAL, 1),
        Theme::success_text("Project created successfully!") | bold | center,
        text("") | size(HEIGHT, EQUAL, 1),
        hbox({
            text("  "),
            vbox({
                hbox({
                    text("Directories: ") | color(TokyoColors::FG),
                    text(std::to_string(dirs_created_.load())) | color(TokyoColors::GREEN) | bold
                }),
                hbox({
                    text("Files: ") | color(TokyoColors::FG),
                    text(std::to_string(files_created_.load())) | color(TokyoColors::GREEN) | bold
                }),
                hbox({
                    text("Time elapsed: ") | color(TokyoColors::FG),
                    text(format_duration(elapsed)) | color(TokyoColors::YELLOW) | bold
                })
            })
        }),
        text("") | size(HEIGHT, EQUAL, 1),
        Theme::horizontal_line()
    });
}

std::string GenerationStats::format_duration(std::chrono::milliseconds ms) const {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(ms);
    auto milliseconds = ms - std::chrono::duration_cast<std::chrono::milliseconds>(seconds);

    std::ostringstream oss;
    if (seconds.count() > 0) {
        oss << seconds.count() << "." << std::setfill('0') << std::setw(2)
            << (milliseconds.count() / 10) << "s";
    } else {
        oss << milliseconds.count() << "ms";
    }
    return oss.str();
}

// ProgressDisplay implementation
ProgressDisplay::ProgressDisplay(
    const std::string& project_name,
    const std::string& template_name,
    int total_items
)
    : project_name_(project_name)
    , template_name_(template_name)
    , progress_bar_(total_items, 40)
    , completed_(false) {
    stats_.start_timer();
}

void ProgressDisplay::update(int current, const std::string& current_item) {
    progress_bar_.update(current);
    progress_bar_.set_current_item(current_item);
}

void ProgressDisplay::increment_file() {
    stats_.increment_files();
}

void ProgressDisplay::increment_directory() {
    stats_.increment_directories();
}

void ProgressDisplay::complete() {
    completed_ = true;
    stats_.stop_timer();
}

Element ProgressDisplay::render() {
    return vbox({
        render_header(),
        text("") | size(HEIGHT, EQUAL, 1),
        progress_bar_.render_with_stats(),
        text("") | size(HEIGHT, EQUAL, 1),
        stats_.render(),
        text("") | size(HEIGHT, EQUAL, 1)
    });
}

Element ProgressDisplay::render_header() {
    return vbox({
        hbox({
            text("Project: ") | color(TokyoColors::COMMENT),
            text(project_name_) | color(TokyoColors::CYAN) | bold
        }),
        hbox({
            text("Template: ") | color(TokyoColors::COMMENT),
            text(template_name_) | color(TokyoColors::MAGENTA) | bold
        })
    });
}

Element ProgressDisplay::render_footer() {
    if (completed_) {
        return stats_.render_summary();
    }
    return text("");
}

Component ProgressDisplay::component() {
    return Renderer([this] {
        return vbox({
            render_header(),
            text("") | size(HEIGHT, EQUAL, 1),
            progress_bar_.render_with_stats(),
            text("") | size(HEIGHT, EQUAL, 1),
            stats_.render(),
            render_footer()
        }) | border | color(TokyoColors::CYAN);
    });
}

// ConsoleProgress implementation
ConsoleProgress::ConsoleProgress(int total, bool verbose)
    : total_(total), current_(0), verbose_(verbose), last_percentage_(-1) {
}

void ConsoleProgress::update(int current, const std::string& item) {
    current_ = current;
    if (verbose_ && !item.empty()) {
        std::cout << "[" << current << "/" << total_ << "] " << item << std::endl;
    } else {
        print_progress();
    }
}

void ConsoleProgress::complete() {
    current_ = total_;
    print_progress();
    std::cout << std::endl;
}

void ConsoleProgress::print_progress() {
    int percentage = (current_ * 100) / total_;
    if (percentage != last_percentage_) {
        std::cout << "\rProgress: " << percentage << "% (" << current_ << "/" << total_ << ")";
        std::cout.flush();
        last_percentage_ = percentage;
    }
}

} // namespace yaqeen::ui
