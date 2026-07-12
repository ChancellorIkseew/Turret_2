#pragma once
#include <cstddef>
#include <cstdint>

namespace util::platform {

    struct MemoryUsage {
        size_t reservedMB;
        size_t usedMB;
    };

    MemoryUsage getMemoryUsage();

    class Console {
        static inline bool visible = false;
    public:
        static void setVisible(const bool flag);
        static bool isVisible() { return visible; }
    };
}
