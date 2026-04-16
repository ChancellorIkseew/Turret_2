#pragma once
#include <filesystem>
#include <string_view>

namespace io {
    namespace fs = std::filesystem;
    enum class Log : uint8_t { only_error, error_and_success };
    ///@brief Can read both text and bin files
    std::string readFile(const fs::path& path, const Log log = Log::error_and_success);
    ///@brief Can write both text and bin files
    bool writeFile(const fs::path& path, const std::string_view text, const Log log = Log::error_and_success);
}
