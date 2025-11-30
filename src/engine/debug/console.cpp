#include "console.hpp"
#ifdef _WIN32
#include <windows.h>

void debug::Console::setVisible(const bool flag) {
    visible = flag;
    HWND hwnd = GetConsoleWindow();
    if (hwnd != nullptr)
        ShowWindow(hwnd, flag ? SW_SHOW : SW_HIDE);
}
#elif defined(__linux__)

void debug::Console::setVisible(const bool flag) {
    //Not needed on Linux.
}
#endif
