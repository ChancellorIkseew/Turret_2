#include "validator.hpp"
//
#include <stdexcept>

std::string val::toStdString(const std::u32string& u32str) {
    if (u32str.empty())
        return "";
    std::string str;
    for (const auto it : u32str) {
        str.push_back(static_cast<char>(it));
    }
    return str;
}

float val::toFloat(const std::string& str) {
    if (str.empty())
        return 0.0f;
    float value = 1.0f;
    try { value = std::stof(str); }
    catch (const std::out_of_range&) { }
    return value;
}

uint64_t val::toUint64(const std::string& str) {
    if (str.empty())
        return 0U;
    uint64_t value = std::numeric_limits<uint64_t>::max();
    try { value = std::stoull(str); }
    catch (const std::out_of_range&) { }
    return value;
}
