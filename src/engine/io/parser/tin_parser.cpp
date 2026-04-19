#include "tin_parser.hpp"
//
#include "engine/debug/logger.hpp"
#include "engine/io/io.hpp"
#include "engine/util/string_util.hpp"

static debug::Logger logger("tin_parser");

std::string tin::writeToString(const Data& data) {
    std::string text;
    text.reserve(data.size() * 32);
    for (const auto& [key, value] : data) {
        text += std::format("{}={}\n", key, value);
    }
    return text;
}

tin::Data tin::readFromString(std::string_view text) {
    tin::Data data;
    while (!text.empty()) {
        const auto line = util::getLine(text);
        if (line.empty() || line.starts_with('#'))
            continue;
        const size_t eqv = line.find('=');
        if (eqv == std::string_view::npos)
            continue;
        const auto key   = line.substr(0, eqv);
        const auto value = line.substr(eqv + 1);
        data.emplace(std::string(key), std::string(value));
    }
    return data;
}

void tin::write(const fs::path& path, const Data& data, const Log log) {
    const std::string text = writeToString(data);
    io::writeFile(path, text);
}

tin::Data tin::read(const fs::path& path, const Log log) {
    const std::string text = io::readFile(path, static_cast<io::Log>(Log::only_error));
    const Data data = readFromString(text);
    if (log == Log::error_and_success)
        logger.info() << "Readen file: " << path;
    return data;
}
