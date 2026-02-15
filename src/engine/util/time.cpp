#include "time.hpp"
//
#include <chrono>
#include <ctime>
#include <sstream>
#include "engine/io/utf8/utf8.hpp"
#pragma warning(disable : 4996)

uint64_t util::time::getLocalTimeSeconds() noexcept {
    using namespace std::chrono;
    auto sec = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    return static_cast<uint64_t>(sec);
}

uint64_t util::time::getLocalTimeMilliseconds() noexcept {
    using namespace std::chrono;
    auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return static_cast<uint64_t>(ms);
}

std::string util::time::yyyyMMddHHmmSS(const uint64_t seconds) {
    time_t tm = static_cast<time_t>(seconds);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&tm), "%Y/%m/%d %T");
    return ss.str();
}

std::u32string util::time::u32yyyyMMddHHmmSS(const uint64_t seconds) {
    return utf8::to_u32string(yyyyMMddHHmmSS(seconds));
}

std::string util::time::timerFormat(const uint64_t seconds) {
    const uint64_t hours = seconds / 3600;
    const uint64_t minutes = (seconds / 60) % 60;
    const uint64_t secondsOnly = seconds % 60;
    std::stringstream ss;
    //
    if (hours > 0)/*------------*/ss << hours << ':';
    //
    if (hours > 0)/*------------*/ss << std::setfill('0') << std::setw(2) << minutes << ':';
    else if (minutes > 0)/*-----*/ss << minutes << ':';
    //
    if (hours > 0 || minutes > 0) ss << std::setfill('0') << std::setw(2) << secondsOnly;
    else/*----------------------*/ss << secondsOnly;
    //
    return ss.str();
}

std::u32string util::time::u32timerFormat(const uint64_t seconds) {
    return utf8::to_u32string(timerFormat(seconds));
}
