#pragma once
#include <array>
#include "engine/coords/pixel_coord.hpp"
#include "engine/render/texture.hpp"
#include "game/blocks/block_archetype.hpp"
#include "game/physics/mob_ai.hpp"
#include "game/physics/physics_base.hpp"
#include "preset_id.hpp"

struct ShellVisualPreset {
    Texture texture;
    PixelCoord origin;
    PixelCoord size;
};

struct ShellPreset {
    float speed;
    Health damage;
    TickCount maxLifeTime;
    Explosion explosion;
    ShellVisualPreset visual;
};

struct TurretVisualPreset {
    Texture texture;
    PixelCoord origin;
    PixelCoord size;
    std::string shotSound;
};

struct TurretPreset {
    TickCount reload;
    float range;
    AngleRad rotationSpeed;
    uint8_t barrelsCount;
    std::array<PixelCoord, 4> barrels;
    ShellPresetID shell;
    TurretVisualPreset visual;
};

struct MobVisualPreset {
    Texture texture;
    PixelCoord origin;
    PixelCoord size;
    uint8_t frameTicks;
    float frameHeight;
    uint8_t frameCount;
    std::array<uint8_t, 16> frameOrder;
};

struct MobPreset {
    float maxSpeed;
    float hitboxRadius;
    Health maxHealth;
    MovingAI defaultMovingAI;
    ShootingAI defaultShootingAI;
    TurretPresetID turret;
    MobVisualPreset visual;
};

struct BlockVisualPreset {
    Texture texture;
    PixelCoord origin;
    PixelCoord size;
    uint8_t frameTicks;
    float frameHeight;
    uint8_t frameCount;
    std::array<uint8_t, 16> frameOrder;
};

struct BlockPreset {
    BlockArchetype archetype;
    Health maxHealth;
    BlockVisualPreset visual;
};
