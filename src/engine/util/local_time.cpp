#include "local_time.hpp"
//
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#pragma warning(disable : 4996)

std::string t1_time::getTime() {
    time_t tm = std::time(nullptr);
    using namespace std::chrono;
    auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) % 1000;
    std::stringstream ss;
    ss << std::put_time(std::localtime(&tm), "%Y/%m/%d %T");
    return ss.str();
}

uint64_t t1_time::getUTC() {
    return static_cast<uint64_t>(std::time(nullptr));
}
