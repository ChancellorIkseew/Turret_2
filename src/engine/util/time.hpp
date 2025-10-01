#pragma once
#include <cstdint>
#include <string>

namespace util::time {
    ///@brief Local time since epoch (in seconds).
    uint64_t getLocalTime();
    ///@brief Local time since epoch (in milliseconds).
    uint64_t getLocalTimeMs();

    ///@brief Seconds to "YYYY/MM/DD HH:MM:SS".
    std::string yyyyMMddHHmmSS(const uint64_t value);
    ///@brief Seconds to "YYYY/MM/DD HH:MM:SS".
    std::u32string u32yyyyMMddHHmmSS(const uint64_t value);
}
