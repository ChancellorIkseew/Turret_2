#pragma once
#include <string>
#include <string_view>

namespace util {
    inline std::string removePrefix(const std::string_view src, const std::string_view prefix) {
        if (src.starts_with(prefix))
            return std::string(src.substr(prefix.size(), src.size()));
        return std::string(src);
    }

    //////@brief Old and new prefix must end with '_' a nave no other '_' symbols.
    inline std::string swapPrefix(const std::string_view src, const std::string_view newPrefix) {
        const size_t prefixLast = src.find_first_of('_');
        if (prefixLast == std::string::npos)
            return "";
        std::string result;
        result += newPrefix;
        result += src.substr(prefixLast + 1, src.size());
        return result;
    }
}

