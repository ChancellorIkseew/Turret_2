#include "time.hpp"
//
#include <chrono>
#include <ctime>
#include <sstream>
#include "engine/io/utf8/utf8.hpp"
#pragma warning(disable : 4996)

uint64_t util::time::getLocalTime() noexcept {
    using namespace std::chrono;
    auto sec = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    return static_cast<uint64_t>(sec);
}

uint64_t util::time::getLocalTimeMs() noexcept {
    using namespace std::chrono;
    auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return static_cast<uint64_t>(ms);
}

std::string util::time::yyyyMMddHHmmSS(const uint64_t value) {
    time_t tm = static_cast<time_t>(value);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&tm), "%Y/%m/%d %T");
    return ss.str();
}

std::u32string util::time::u32yyyyMMddHHmmSS(const uint64_t value) {
    return utf8::to_u32string(yyyyMMddHHmmSS(value));
}
