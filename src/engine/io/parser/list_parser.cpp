#include "list_parser.hpp"
//
#include "engine/debug/logger.hpp"
#include "engine/io/io.hpp"
#include "engine/util/string_util.hpp"

namespace fs = std::filesystem;
static debug::Logger logger("list_parser");

void list::write(fs::path path, const list::Data& data) {
    std::string text;
    text.reserve(data.size() * 16);
    for (const auto& str : data) {
        text += str + '\n';
    }
    io::writeFile(path, text);
}

list::Data list::read(fs::path path) {
    const std::string text = io::readFile(path, io::Log::only_error);
    std::string_view rest(text);
    list::Data data;
    while (!rest.empty()) {
        const auto line = util::getLine(rest);
        if (!line.empty() && line[0] != '#')
            data.push_back(std::string(line));
    }
    logger.info() << "Readen file: " << path;
    return data;
}
