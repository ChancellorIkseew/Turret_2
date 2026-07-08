#pragma once

namespace util::platform {

    struct MemoryUsage {
        size_t reservedMB;
        size_t usedMB;
    };

    MemoryUsage getMemoryUsage();
}
