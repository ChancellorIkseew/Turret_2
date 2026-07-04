#include "shells_system.hpp"
//
#include <cmath>
#include "engine/assets/presets.hpp"
#include "engine/render/renderer.hpp"
#include "game/blocks/block_map.hpp"
#include "game/player/camera.hpp"
#include "game/entities/chunk_grid.hpp"
#include "game/entities/mobs_pool.hpp"
#include "game/entities/shells_pool.hpp"

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

void shells::processShells(ShellSoA& shells, MobSoA& mobs, const ChunkGrid& chunks, BlockMap& blocks) {
    const size_t shellCount = shells.shellCount;
    reduceShellsLifeTime(shells);
    moveShells(shells, shellCount);
    hitMobs(shells, mobs, shellCount, chunks);
    hitBlocks(shells, blocks, shellCount);
}

void shells::cleanupShells(ShellsPool& shellsPool, const Presets& presets/*, Explosions& explosions*/) {
    const auto& soa = shellsPool.getSoa();
    // Reverse itaretion to avoid bugs with "swap and pop".
    for (size_t i = soa.shellCount; i > 0; --i) {
        size_t index = i - 1;
        if (soa.restLifeTime[index] > 0 && soa.restDamage[index] > 0)
            continue;
        //if (soa.presets->explosion.damage != 0)
        //     explosions.push(soa.presets->explosion);
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
