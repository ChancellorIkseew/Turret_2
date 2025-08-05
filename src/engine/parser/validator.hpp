#pragma once
#include <cstdint>
#include <optional>
#include <string>

namespace val {
    std::string toStdString(const std::u32string& str);
    //
    float toFloat(const std::string& str);
    uint64_t toUint64(const std::string& str);






}
