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
#include <unistd.h>

void debug::Console::setVisible(const bool flag) {
    visible = flag;
    if (visible) {
    } // TODO: find imlementation for Linux
    else
        daemon(0, 0);
}
#endif
