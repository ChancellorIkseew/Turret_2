#include "platform.hpp"
//
#ifdef __linux__
#include <fstream>
#include <string>
#include <unistd.h>

namespace plt = util::platform;

plt::MemoryUsage plt::getMemoryUsage() {
    std::ifstream statm("/proc/self/statm");
    if (statm.is_open()) {
        unsigned long vmPages, rssPages;
        statm >> vmPages >> rssPages;
        long pageSizeBytes = sysconf(_SC_PAGE_SIZE);
        //
        const size_t usedMB =    (rssPages * pageSizeBytes);
        const size_t reservedMB = (vmPages * pageSizeBytes);
        return plt::MemoryUsage(reservedMB, usedMB);
    }
    return plt::MemoryUsage(0, 0);
}

void plt::Console::setVisible(const bool flag) {
    //Not needed on Linux.
}

#endif
