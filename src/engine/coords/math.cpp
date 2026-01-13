#include "math.hpp"
//
#include <algorithm> // std::max is placed <utility> in MSVC, but <algorithm> in GCC

int t1::ceilUpPow2(const int value) noexcept {
    int val = value;
    --val;
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    ++val;
    return std::max(val, 1);
}
