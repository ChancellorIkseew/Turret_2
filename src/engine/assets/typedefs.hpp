#pragma once
#include <cstdint>

using TeamID = uint8_t;
using Health = int64_t;
using AngleRad = float;
using TickCount = uint16_t;

enum class MovingAI : uint8_t {
    none,
    basic,
    builder,
    player_controlled
};

enum class ShootingAI : uint8_t {
    none,
    basic,
    player_controlled
};

// order of types does mater
enum class BlockType {
    air,
    in_progress,
    wall,
    link,
    belt,
    bridge,
    drill,
    impact_drill,
    factory,
    junction,
    router,
    turret,
    core
};

namespace cl {
    inline constexpr uint32_t fading(const TickCount lifeTime, const uint8_t startAlpha = 0xFF) {
        return uint32_t(float(startAlpha) / float(lifeTime));
    }

    constexpr uint32_t TR = 0xFF'FF'FF'00;
    constexpr uint32_t RED = 0x84'34'34'FF;
    constexpr uint32_t BEIGE = 0xFA'DC'86'FF;
    constexpr uint32_t ORANGE = 0xFF'A5'00'FF;
    constexpr uint32_t CYAN = 0x80'FF'FF'FF;
    constexpr uint32_t RED_TR = RED & TR;
    constexpr uint32_t BEIGE_TR = BEIGE & TR;
    constexpr uint32_t SMOKE = 0xC0'C0'C0'FF;
    constexpr uint32_t SHADOW = 0x00'00'00'40;
}
