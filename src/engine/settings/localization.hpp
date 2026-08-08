#pragma once
#include <format>
#include <optional>
#include <string>
#include <unordered_map>
#include "engine/io/parser/tin_parser.hpp"

class Localization {
    static inline std::unordered_map<std::string, std::string> dictionary;
public:
    static void load(std::unordered_map<std::string, std::string>&& dictionary) {
        Localization::dictionary = dictionary;
    }
    //
    static std::optional<std::string> get(const std::string key) {
        if (dictionary.contains(key)) return dictionary.at(key);
        return std::nullopt;
    }
};

inline std::string tr(const std::string& key) {
    return Localization::get(key).value_or(key);
}

template <class... Args>
inline std::string tr(const std::string& key, Args&&... args) {
    const std::string format = Localization::get(key).value_or(key);
    return std::vformat(format, std::make_format_args(args...));
}
