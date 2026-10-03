#pragma once
#include <format>
#include "config.hpp"

namespace debug {
    enum class LogLevel : uint8_t { attention, debug, info, warning, error };

    class Logger {
        const std::string name;
    private:
        void logFormated(LogLevel level, std::string_view fmt, std::format_args args) const;
        void log(LogLevel level, std::string_view message) const;
        t1_disable_copy_and_move(Logger)
    public:
        explicit Logger(std::string name) : name(std::move(name)) {}
        static void init(const std::string& filename);
        //
        class LogMessage {
            const Logger* logger;
            LogLevel level;
        public:
            LogMessage(const Logger* logger, LogLevel level) : logger(logger), level(level) {}
            //
            template <class... Args> requires (sizeof...(Args) > 0)
            void operator()(std::format_string<Args...> fmt, Args&&... args) const {
                logger->logFormated(level, fmt.get(), std::make_format_args(args...));
            }
            void operator()(std::string_view msg) const {
                logger->log(level, msg);
            }
        };
        //
        const LogMessage attention = LogMessage(this, LogLevel::attention);
        const LogMessage debug     = LogMessage(this, LogLevel::debug);
        const LogMessage info      = LogMessage(this, LogLevel::info);
        const LogMessage warning   = LogMessage(this, LogLevel::warning);
        const LogMessage error     = LogMessage(this, LogLevel::error);
    };
}
