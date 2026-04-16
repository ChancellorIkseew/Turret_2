#include "time.hpp"
//
#include <chrono>

using namespace std::chrono;

uint64_t util::time::getLocalTimeSeconds() noexcept {
    const auto sec = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    return static_cast<uint64_t>(sec);
}

uint64_t util::time::getLocalTimeMilliseconds() noexcept {
    const auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return static_cast<uint64_t>(ms);
}

std::string util::time::yyyyMMddHHmmSS(const uint64_t seconds) {
    const sys_seconds timePoint{ std::chrono::seconds(seconds) };
    const local_seconds local = std::chrono::current_zone()->to_local(timePoint);
    return std::format("{:%Y/%m/%d %T}", local);
}

std::string util::time::timerFormat(const uint64_t seconds) {
    const uint64_t hours = seconds / 3600;
    const uint64_t minutes = (seconds / 60) % 60;
    const uint64_t secondsOnly = seconds % 60;
    if (hours > 0)
        return std::format("{}:{:02}:{:02}", hours, minutes, secondsOnly);
    if (minutes > 0)
        return std::format("{}:{:02}", minutes, secondsOnly);
    return std::format("{}", secondsOnly);
}
