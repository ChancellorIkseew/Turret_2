#pragma once

namespace debug {
    class Console {
        static inline bool visible = false;
    public:
        static void setVisible(const bool flag);
        static bool isVisible() { return visible; }
    };
}
