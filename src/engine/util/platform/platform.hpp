#pragma once
#include <cstddef>
#include <cstdint>

namespace util::platform {

    struct MemoryUsage {
        size_t reservedMB;
        size_t usedMB;
    };

    MemoryUsage getMemoryUsage();
}
