#pragma once
#include <string>
#include <unordered_map>
#include "ui_config.hpp"

START_NAMESPACE_MINGUI

class Localization {
    std::unordered_map<std::string, std::string> dictionary;
public:
    Localization(std::unordered_map<std::string, std::string>&& dictionary) :
        dictionary(dictionary) { }
    ///@brief Throws out_of_range.
    std::string at(const std::string& key) const {
        return dictionary.at(key);
    }
    bool contains(const std::string& key) const {
        return dictionary.contains(key);
    }
};

END_NAMESPACE_MINGUI
