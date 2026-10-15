#pragma once
#include <string>
#include <string_view>

namespace util {
    inline std::string removePrefix(const std::string_view src, const std::string_view prefix) {
        if (src.starts_with(prefix))
            return std::string(src.substr(prefix.size(), src.size()));
        return std::string(src);
    }

    inline constexpr std::string_view getLine(std::string_view& src /*mutable ref*/) {
        const size_t endLine = src.find_first_of("\r\n");
        const std::string_view line = src.substr(0, endLine);
        if (endLine == std::string_view::npos) {
            src = {};
            return line;
        }
        src.remove_prefix(endLine);
        src.remove_prefix((src.size() > 1 && src[0] == '\r' && src[1] == '\n') ? 2 : 1);
        return line;
    }
}
