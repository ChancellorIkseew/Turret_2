#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include "ui_config.hpp"

START_NAMESPACE_MINGUI

class Localization {
    std::unordered_map<std::string, std::string> dictionary;
public:
    Localization(std::unordered_map<std::string, std::string>&& dictionary) :
        dictionary(dictionary) { }
    //
    std::optional<std::string> get(const std::string& key) const {
        if (dictionary.contains(key)) return dictionary.at(key);
        return std::nullopt;
    }
};

END_NAMESPACE_MINGUI
