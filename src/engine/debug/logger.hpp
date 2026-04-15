#pragma once
#include <cstdint>
#include <sstream>

namespace debug {
    enum class LogLevel : uint8_t { attention, debug, info, warning, error };
    class Logger;

    class LogMessage {
        Logger* logger;
        LogLevel level;
        std::stringstream ss;
    public:
        LogMessage(Logger* logger, LogLevel level) : logger(logger), level(level) { }
        ~LogMessage();

        template <class T>
        LogMessage& operator<<(const T& x) {
            ss << x;
            return *this;
        }
    };

    class Logger {
        const std::string name;
    public:
        Logger(std::string name) : name(std::move(name)) { }
        void log(LogLevel level, const std::string& message) const;
        //
        static void init(const std::string& filename);
        //
        LogMessage attention() { return LogMessage(this, LogLevel::attention); }
        LogMessage debug()     { return LogMessage(this, LogLevel::debug); }
        LogMessage info()      { return LogMessage(this, LogLevel::info); }
        LogMessage warning()   { return LogMessage(this, LogLevel::warning); }
        LogMessage error()     { return LogMessage(this, LogLevel::error); }
    };
}
