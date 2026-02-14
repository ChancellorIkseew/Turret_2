#pragma once
#include "tin_parser.hpp"

namespace jtin {
    using Data = std::vector<tin::Data>;
    void write(std::ostream& text, const Data& data);
    Data read(std::istream& text);
    namespace fs = std::filesystem;
    enum class Log : uint8_t { only_error, error_and_success };
    void write(const fs::path& path, const Data& data, const Log log = Log::error_and_success);
    Data read(const fs::path& path, const Log log = Log::error_and_success);
}
