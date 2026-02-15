#pragma once
#include <cstdint>
#include <string>

namespace util::time {
    ///@brief Local time since epoch.
    uint64_t getLocalTimeSeconds() noexcept;
    ///@brief Local time since epoch.
    uint64_t getLocalTimeMilliseconds() noexcept;

    ///@brief Seconds to "YYYY/MM/DD HH:MM:SS".
    std::string yyyyMMddHHmmSS(const uint64_t seconds);
    ///@brief Seconds to "YYYY/MM/DD HH:MM:SS".
    std::u32string u32yyyyMMddHHmmSS(const uint64_t seconds);

    ///@brief Seconds to "HH:MM:SS". Mindustry like timer format.
    std::string timerFormat(const uint64_t seconds);
    ///@brief Seconds to "HH:MM:SS". Mindustry like timer format.
    std::u32string u32timerFormat(const uint64_t seconds);
}
