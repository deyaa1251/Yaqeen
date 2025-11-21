#include "yaqeen/utils/logger.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

namespace yaqeen {

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger()
    : min_level_(LogLevel::Info)
    , console_enabled_(true) {
}

Logger::~Logger() {
    if (file_stream_) {
        file_stream_->close();
    }
}

void Logger::set_level(LogLevel level) {
    min_level_ = level;
}

void Logger::set_output_file(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    file_stream_ = std::make_unique<std::ofstream>(filename, std::ios::app);
}

void Logger::enable_console(bool enable) {
    console_enabled_ = enable;
}

void Logger::debug(std::string_view message) {
    log(LogLevel::Debug, message);
}

void Logger::info(std::string_view message) {
    log(LogLevel::Info, message);
}

void Logger::warn(std::string_view message) {
    log(LogLevel::Warn, message);
}

void Logger::error(std::string_view message) {
    log(LogLevel::Error, message);
}

void Logger::log(LogLevel level, std::string_view message) {
    if (level < min_level_) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    std::ostringstream oss;
    oss << "[" << format_timestamp() << "] "
        << "[" << level_to_string(level) << "] "
        << message;

    std::string formatted = oss.str();

    if (console_enabled_) {
        if (level == LogLevel::Error) {
            std::cerr << formatted << std::endl;
        } else {
            std::cout << formatted << std::endl;
        }
    }

    if (file_stream_ && file_stream_->is_open()) {
        *file_stream_ << formatted << std::endl;
    }
}

std::string Logger::format_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

const char* Logger::level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info:  return "INFO ";
        case LogLevel::Warn:  return "WARN ";
        case LogLevel::Error: return "ERROR";
        default: return "UNKNOWN";
    }
}

} // namespace yaqeen
