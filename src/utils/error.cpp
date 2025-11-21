#include "yaqeen/utils/error.hpp"
#include <sstream>

namespace yaqeen {

const char* Error::code_to_string() const {
    switch (code) {
        case ErrorCode::FileNotFound: return "FileNotFound";
        case ErrorCode::FileAlreadyExists: return "FileAlreadyExists";
        case ErrorCode::PermissionDenied: return "PermissionDenied";
        case ErrorCode::DirectoryNotFound: return "DirectoryNotFound";
        case ErrorCode::CannotCreateDirectory: return "CannotCreateDirectory";
        case ErrorCode::CannotCreateFile: return "CannotCreateFile";
        case ErrorCode::InvalidMarkdownFormat: return "InvalidMarkdownFormat";
        case ErrorCode::InvalidJSONFormat: return "InvalidJSONFormat";
        case ErrorCode::InvalidTemplateStructure: return "InvalidTemplateStructure";
        case ErrorCode::ParsingFailed: return "ParsingFailed";
        case ErrorCode::TemplateNotFound: return "TemplateNotFound";
        case ErrorCode::TemplateLoadFailed: return "TemplateLoadFailed";
        case ErrorCode::TemplateInvalid: return "TemplateInvalid";
        case ErrorCode::InvalidInput: return "InvalidInput";
        case ErrorCode::UnknownError: return "UnknownError";
        default: return "Unknown";
    }
}

std::string Error::to_string() const {
    std::ostringstream oss;
    oss << "[" << code_to_string() << "] " << message;
    if (details.has_value() && !details->empty()) {
        oss << "\n  Details: " << *details;
    }
    return oss.str();
}

} // namespace yaqeen
