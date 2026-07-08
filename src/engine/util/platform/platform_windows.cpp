#include "platform.hpp"
//
#ifdef _WIN32
#include <windows.h>
#include <psapi.h> // after windows.h

namespace plt = util::platform;

plt::MemoryUsage plt::getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc{};
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        const size_t usedMB =   pmc.WorkingSetSize / 1024 / 1024;
        const size_t reservedMB = pmc.PrivateUsage / 1024 / 1024;
        return plt::MemoryUsage(reservedMB, usedMB);
    }
    return plt::MemoryUsage(0, 0);
}

#endif
