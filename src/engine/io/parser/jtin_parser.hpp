#pragma once
#include "tin_parser.hpp"

namespace jtin {
    using Data = std::vector<tin::Data>;
    std::string writeToString(const Data& data);
    Data readFromString(std::string_view text);
    namespace fs = std::filesystem;
    void write(const fs::path& path, const Data& data, const io::Log log = io::Log::error_and_success);
    Data read(const fs::path& path, const io::Log log = io::Log::error_and_success);
}
