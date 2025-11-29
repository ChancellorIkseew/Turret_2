#include <iostream>
#include <string>
#include "logger.hpp"

#ifdef _WIN32
#include <windows.h>

namespace debug {
    constexpr WORD RED = FOREGROUND_RED;
    constexpr WORD YELLOW = FOREGROUND_RED | FOREGROUND_GREEN;
    constexpr WORD DEFAULT = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    static void printColorfull(const std::string& string, const WORD color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        SetConsoleTextAttribute(hConsole, color | (csbi.wAttributes & 0xFFF0));
        std::cout << string << std::endl;
        SetConsoleTextAttribute(hConsole, DEFAULT | (csbi.wAttributes & 0xFFF0));
    }

    static void printColorfull(const std::string& string, const LogLevel level) {
        switch (level) {
        case LogLevel::warning:
            printColorfull(string, YELLOW);
            break;
        case LogLevel::error:
            printColorfull(string, RED);
            break;
        default:
            std::cout << string << std::endl;
            break;
        }
    }
}
#elif defined(__linux__)
namespace debug {
    constexpr std::string_view RED = "\033[31m";
    constexpr std::string_view YELLOW = "\033[33m";
    constexpr std::string_view DEFAULT = "\033[0m";

    static void printColorfull(const std::string& string, const LogLevel level) {
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
#endif
