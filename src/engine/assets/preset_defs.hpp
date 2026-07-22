#pragma once
#include <array>
#include "engine/coords/pixel_coord.hpp"
#include "engine/render/texture_rect.hpp"
#include "game/blocks/block.hpp"
#include "game/common/mob_ai.hpp"
#include "game/common/physics_base.hpp"
#include "preset_id.hpp"

struct ItemPreset {
    TextureRect textureRect;
};

struct OrePreset {
    ItemPresetID item;
    std::string visibleName;
    TextureRect textureRect;
};

struct ShellVisualPreset {
    TextureRect textureRect;
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
    TextureRect textureRect;
    PixelCoord origin;
    PixelCoord size;
    float shadowOffset;
    std::string shotSound;
};

struct TurretPreset {
    TickCount reload;
    float range;
    float recoil;
    AngleRad spreadRad;
    AngleRad rotationSpeed;
    uint8_t barrelsCount;
    std::array<PixelCoord, 4> barrels;
    ShellPresetID shell;
    TurretVisualPreset visual;
};

struct MobVisualPreset {
    TextureRect textureRect;
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
    float shieldRadius;
    Health maxHealth;
    Health maxShieldHealth;
    bool flying;
    bool canBuild;
    uint8_t buildSpeed;
    MovingAI defaultMovingAI;
    ShootingAI defaultShootingAI;
    TurretPresetID turret;
    MobVisualPreset visual;
};

struct BlockVisualPreset {
    TextureRect textureRect;
    uint8_t frameTicks;
    float frameHeight;
    uint8_t frameCount;
    std::array<uint8_t, 16> frameOrder;
};

struct Ingredient {
    ItemPresetID itemID;
    uint16_t amount;
};

struct BlockPreset {
    bool rotatable;
    uint8_t size;
    BlockType archetype;
    Health maxHealth;
    BlockVisualPreset visual;
    union {
        TurretPresetID turret;
    };
    std::array<Ingredient, 6> recipe;
};
