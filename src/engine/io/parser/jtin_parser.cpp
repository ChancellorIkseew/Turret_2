#include "jtin_parser.hpp"
//
#include "engine/debug/logger.hpp"

static debug::Logger logger("jtin_parser");

std::string jtin::writeToString(const Data& data) {
    std::string text;
    for (const tin::Data& block : data) {
        text += "{\n";
        text += tin::writeToString(block);
        text += "}\n";
    }
    return text;
}

jtin::Data jtin::readFromString(std::string_view text) {
    jtin::Data data;
    while (!text.empty()) {
        const size_t startBlock = text.find_first_of('{');
        const size_t endBlock   = text.find_first_of('}');
        if (startBlock == std::string_view::npos || endBlock == std::string_view::npos)
            break;
        if (endBlock <= startBlock) {
            text.remove_prefix(endBlock + 1);
            continue;
        }
        data.push_back(tin::readFromString(text.substr(startBlock + 1, endBlock - 1)));
        text.remove_prefix(endBlock + 1);
    }
    return data;
}

void jtin::write(const fs::path& path, const jtin::Data& data, const io::Log log) {
    std::string text = writeToString(data);
    io::writeFile(path, text, log);
}

jtin::Data jtin::read(const fs::path& path, const io::Log log) {
    const std::string text = io::readFile(path, log);
    const Data data = readFromString(text);
    if (log == io::Log::error_and_success)
        logger.info("Parsed file: {}", path.string());
    return data;
}
