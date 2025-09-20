#pragma once
#include "config.hpp"

namespace t1 {
    constexpr float PI_F = 3.1415927f;
    constexpr double PI_D = 3.1415926535897931;

    t1_finline float radToDegree(const float radian) noexcept {
        return 180.0f - radian * (180.0f / PI_F);
    }
    t1_finline double radToDegree(const double radian) noexcept {
        return 180.0f - radian * (180.0f / PI_D);
    }

    t1_finline int pow2i(const int value) noexcept {
        return value * value;
    }
    t1_finline float pow2f(const float value) noexcept {
        return value * value;
    }
    t1_finline double pow2d(const double value) noexcept {
        return value * value;
    }

    int ceilUpPow2(const int value) noexcept;
}
