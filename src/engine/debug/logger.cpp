#include "logger.hpp"
//
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <mutex>
#include <utility>
#pragma warning(disable : 4996)

using namespace debug;
static std::ofstream fout;
static std::mutex mutex;
static std::string utcOffset = "";
constexpr std::string_view RED = "\033[31m";
constexpr std::string_view YELLOW = "\033[33m";
constexpr std::string_view DEFAULT = "\033[0m";
constexpr unsigned int moduleLen = 20;

LogMessage::~LogMessage() {
    logger->log(level, ss.str());
}

static void write(LogLevel level, const std::string& name, const std::string& message) {
    std::stringstream ss;
    switch (level) {
    case LogLevel::debug:
#ifndef NDEBUG
        ss << "[D]";
        break;
#endif
        return;
    case LogLevel::info:
        ss << "[I]";
        break;
    case LogLevel::warning:
        ss << "[W]";
        break;
    case LogLevel::error:
        ss << "[E]";
        break;
    }

    time_t tm = std::time(nullptr);
    using namespace std::chrono;
    auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) % 1000;
    ss << " " << std::put_time(std::localtime(&tm), "%Y/%m/%d %T");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    ss << utcOffset << " [" << std::setfill(' ') << std::setw(moduleLen) << name << "] ";
    ss << message;
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto string = ss.str();
        if (fout.good()) {
            fout << string << '\n';
            fout.flush();
        }
        switch (level) {
        case LogLevel::warning:
            std::cout << YELLOW << string << DEFAULT << std::endl;
            break;
        case LogLevel::error:
            std::cout << RED << string << DEFAULT << std::endl;
            break;
        default: 
            std::cout << string << std::endl;
            break;
        }
    }
}

void Logger::init(const std::string& filename) {
    fout.open(filename);

    time_t tm = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&tm), "%z");
    utcOffset = ss.str();
}

void Logger::log(LogLevel level, const std::string& message) const {
    write(level, name, message);
}
