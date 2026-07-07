#include "shells_system.hpp"
//
#include <cmath>
#include "engine/assets/presets.hpp"
#include "engine/audio/sound_queue.hpp"
#include "engine/render/renderer.hpp"
#include "engine/util/fast_rand.hpp"
#include "game/player/camera.hpp"
#include "game/world/world.hpp"

static inline void reduceShellsLifeTime(ShellSoA& soa) {
    for (auto& time : soa.restLifeTime) {
        --time;
    }
}

static inline void moveShells(ShellSoA& soa, const size_t shellCount) {
    for (size_t i = 0; i < shellCount; ++i) {
        soa.position[i] += soa.velocity[i];
    }
}

static t1_finline_cxpr void registerDamage(Health& a, Health& b) {
    const Health takenDamage = std::min(a, b);
    a -= takenDamage;
    b -= takenDamage;
}

static inline void hitMobs(ShellSoA& shells, MobSoA& mobs, const size_t shellCount, const ChunkGrid& chunks) {
    for (size_t shell = 0; shell < shellCount; ++shell) {
        if (shells.restDamage[shell] < 1)
            continue;
        const PixelCoord shellPosition = shells.position[shell];
        for (auto mob : chunks.getChunk(shellPosition)) {
            if (mobs.teamID[mob] == shells.teamID[shell])
                continue;
            if (!CircleHitbox(mobs.position[mob], mobs.hitboxRadius[mob]).contains(shellPosition))
                continue;
            if (mobs.shieldHealth[mob] > 0)
                registerDamage(mobs.shieldHealth[mob], shells.restDamage[shell]);
            else
               registerDamage(mobs.health[mob], shells.restDamage[shell]);
            if (shells.restDamage[shell] < 1)
                break;
        }
    }
}

static inline void hitBlocks(ShellSoA& shells, BlockMap& blocks, const size_t shellCount) {
    for (size_t shell = 0; shell < shellCount; ++shell) {
        if (shells.restDamage[shell] < 1)
            continue;
        const TileCoord tile = t1::tile(shells.position[shell]);
        if (!blocks.contains(tile))
            continue;
        BlockTile& blockTile = blocks.at(tile);
        if (blockTile.type == BlockType::air)
            continue;
        if (blockTile.teamID == shells.teamID[shell])
            continue;
        registerDamage(blockTile.block->health, shells.restDamage[shell]);
        if (blockTile.block->health < 1)
            blocks.demolish(tile);
    }
}

static void finalizeShells(ShellsPool& shellsPool, ParticlesPool& particlesPool, const Presets& presets,
    SoundQueue& sounds, const Camera& camera, const size_t shellsCount) {
    const auto& soa = shellsPool.getSoa();
    for (size_t i = 0; i < shellsCount; ++i) {
        if (soa.restLifeTime[i] > 0 && soa.restDamage[i] > 0)
            continue;
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        const ShellPreset& preset = presets.getShell(soa.preset[i]);
        if (preset.visual.size.y > 6.f) {
            sounds.pushSound("shell_explosion", soa.position[i]);
        }
        constexpr float SPEED = 0.8f;
        const float radius = preset.explosion.radius;
        const PixelCoord size(radius, radius);
        const TickCount lifeTime = static_cast<TickCount>(radius / SPEED);
        for (int j = 0; j < 8; ++j) {
            const float angle = soa.angle[i] + t1::TAU * 0.5f * static_cast<float>(j);
            particlesPool.addParticle(soa.position[i], size, angle, SPEED, 0xFF'A5'00'FF, 0, lifeTime, PType::light);
        }

        const int shardsCount = preset.explosion.shardsCount;
        const PixelCoord shardSize(1.0f, preset.visual.size.y);
        for (int j = 0; j < shardsCount; ++j) {
            const float angle = util::randAngleRad(static_cast<uint32_t>(j * i));
            particlesPool.addParticle(soa.position[i], shardSize, angle, SPEED, 0xFF'A5'00'FF, 0, lifeTime * 2, PType::shard);
        }
    }
}

void shells::processShells(World& world, const Presets& presets, SoundQueue& sounds, const Camera& camera) {
    ShellsPool& shellsPool = world.getShells();
    ShellSoA& shells = shellsPool.getSoa();
    BlockMap& blocks = world.getBlocks();
    const ChunkGrid& chunks = world.getChunks();
    MobSoA& mobs = world.getMobs().getSoa();
    ParticlesPool& particlesPool = world.getParticles();

    const size_t shellCount = shells.shellCount;
    reduceShellsLifeTime(shells);
    moveShells(shells, shellCount);
    hitMobs(shells, mobs, shellCount, chunks);
    hitBlocks(shells, blocks, shellCount);
    finalizeShells(shellsPool, particlesPool, presets, sounds, camera, shellCount);
}

void shells::cleanupShells(ShellsPool& shellsPool, const Presets& presets) {
    const auto& soa = shellsPool.getSoa();
    // Reverse itaretion to avoid bugs with "swap and pop".
    for (size_t i = soa.shellCount; i > 0; --i) {
        size_t index = i - 1;
        if (soa.restLifeTime[index] > 0 && soa.restDamage[index] > 0)
            continue;
        shellsPool.removeShell(index);
    }
}

void shells::drawShells(const ShellSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer) {
    const size_t shellCount = soa.shellCount;
    for (size_t i = 0; i < shellCount; ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        auto& visual = presets.getShell(soa.preset[i]).visual;
        renderer.draw(visual.textureRect, soa.position[i], visual.size, visual.origin, t1::PI - soa.angle[i]);
    }
}

void shells::drawShellsLighting(const ShellSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer) {
    const size_t shellCount = soa.shellCount;
    constexpr TextureRect rect{ 0.f, 0.f, 1.f, 1.f };
    const float scale = camera.getMapScale();
    const PixelCoord translation = camera.getTranslation();

    for (size_t i = 0; i < shellCount; ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        const auto& visual = presets.getShell(soa.preset[i]).visual;
        renderer.draw(rect, soa.position[i], visual.size * 8.f, visual.origin * 8.f, t1::PI - soa.angle[i], 0xFF'A5'00'FF);
    }
}
