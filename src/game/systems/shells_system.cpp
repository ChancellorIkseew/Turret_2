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

static inline void makeTrails(const ShellSoA& soa, ParticlesPool& particlesPool, const Presets& presets,
    const Camera& camera, const size_t shellCount) {
    for (size_t i = 0; i < shellCount; ++i) {
        constexpr PixelCoord SIZE(3, 3);
        constexpr uint32_t FADING = uint32_t(float(0xFF) / 20.f);
        if (camera.contains(soa.position[i]) && presets.getShell(soa.preset[i]).visual.hasTrail)
            particlesPool.addParticle(soa.position[i], SIZE, 0.f, 0.f, cl::ORANGE, FADING, 20, PType::light);
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
        const TileCoord masterTile = blocks.getMaster(tile);
        registerDamage(blocks.at(masterTile).block->health, shells.restDamage[shell]);
        if (blocks.at(masterTile).block->health < 1)
            blocks.demolish(masterTile);
    }
}

static void finalizeShells(ShellsPool& shellsPool, ParticlesPool& particlesPool, MobSoA& mobs, BlockMap& blocks,
    const Presets& presets, SoundQueue& sounds, const Camera& camera, const size_t shellsCount, const uint64_t tickCount) {
    const auto& soa = shellsPool.getSoa();
    for (size_t i = 0; i < shellsCount; ++i) {
        if (soa.restLifeTime[i] > 0 && soa.restDamage[i] > 0)
            continue;
        const ShellPreset& preset = presets.getShell(soa.preset[i]);
        const Health damage = preset.explosion.damage;
        if (damage > 0) {
            const float sqRadius = t1::pow2f(preset.explosion.radius);
            const TeamID teamID = soa.teamID[i];
            const PixelCoord position = soa.position[i];

            //to mob
            for (size_t mob = 0; mob < mobs.mobCount; ++mob) {
                if (teamID == mobs.teamID[mob] || t1::squareDistance(position, mobs.position[mob]) > sqRadius)
                    continue;
                if (mobs.shieldHealth[mob] > 0)
                    mobs.shieldHealth[mob] = std::max<Health>(0, mobs.shieldHealth[mob] - damage);
                else
                    mobs.health[mob] = std::max<Health>(0, mobs.health[mob] - damage);
            }

            //to block
            const int tileRange = t1::tile(preset.explosion.radius);
            const TileCoord tilePosition = t1::tile(position);
            const TileCoord mapSize = blocks.getSize();
            const int startX = std::max(0, tilePosition.x - tileRange);
            const int startY = std::max(0, tilePosition.y - tileRange);
            const int endX = std::min(mapSize.x, tilePosition.x + tileRange);
            const int endY = std::min(mapSize.y, tilePosition.y + tileRange);
            for (int x = startX; x < endX; ++x) {
                for (int y = startY; y < endY; ++y) {
                    const BlockTile& blockTile = blocks.at(x, y);
                    if (blockTile.type == BlockType::air)
                        continue;
                    if (blockTile.teamID == teamID)
                        continue;
                    const PixelCoord blockCenter = t1::tileCenter({ x, y });
                    const float sqDistance = t1::squareDistance(position, blockCenter);
                    if (sqDistance < sqRadius) {
                        const TileCoord masterTile = blocks.getMaster(TileCoord(x, y));
                        blocks.at(masterTile).block->health = std::max<Health>(0, blocks.at(masterTile).block->health - damage);
                        if (blocks.at(masterTile).block->health < 1)
                            blocks.demolish(masterTile);
                    }
                }
            }
        }
        if (!camera.contains(soa.position[i]))
            continue;
        if (preset.visual.size.y > 6.f)
            sounds.pushSound("shell_explosion", soa.position[i]);
        constexpr float SPEED = 0.8f;
        const float radius = preset.explosion.radius;
        const PixelCoord size(radius, radius);
        const TickCount lifeTime = static_cast<TickCount>(radius / SPEED);
        for (int j = 0; j < 8; ++j) {
            const float angle = soa.angle[i] + t1::TAU * 0.5f * static_cast<float>(j);
            const uint32_t ALPHA = 0xFF'FF'FF'80;
            particlesPool.addParticle(soa.position[i], size, angle, SPEED, cl::ORANGE & ALPHA, 0, lifeTime, PType::light);
        }

        const int shardsCount = preset.explosion.shardsCount;
        const PixelCoord shardSize(1.0f, preset.visual.size.y);
        for (int j = 0; j < shardsCount; ++j) {
            const float angle = util::randAngleRad(static_cast<uint32_t>(j * (tickCount % 255)));
            particlesPool.addParticle(soa.position[i], shardSize, angle, SPEED, cl::ORANGE, 0, lifeTime * 2, PType::shard);
        }
    }
}

void shells::processShells(World& world, const Presets& presets, SoundQueue& sounds, const Camera& camera, const uint64_t tickCount) {
    ShellsPool& shellsPool = world.getShells();
    ShellSoA& shells = shellsPool.getSoa();
    BlockMap& blocks = world.getBlocks();
    const ChunkGrid& chunks = world.getChunks();
    MobSoA& mobs = world.getMobs().getSoa();
    ParticlesPool& particlesPool = world.getParticles();

    const size_t shellCount = shells.shellCount;
    reduceShellsLifeTime(shells);
    moveShells(shells, shellCount);
    makeTrails(shells, particlesPool, presets, camera, shellCount);
    hitMobs(shells, mobs, shellCount, chunks);
    hitBlocks(shells, blocks, shellCount);
    finalizeShells(shellsPool, particlesPool, mobs, blocks, presets, sounds, camera, shellCount, tickCount);
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
        if (!camera.contains(soa.position[i]))
            continue;
        auto& visual = presets.getShell(soa.preset[i]).visual;
        renderer.draw(visual.textureRect, soa.position[i], visual.size, visual.origin, t1::PI - soa.angle[i]);
    }
}

void shells::drawShellsLighting(const ShellSoA& soa, const Presets& presets, const Camera& camera, Renderer& renderer) {
    const size_t shellCount = soa.shellCount;
    constexpr TextureRect rect{ 0.f, 0.f, 1.f, 1.f };
    constexpr PixelCoord OFFSET(4.f, 8.f);

    for (size_t i = 0; i < shellCount; ++i) {
        if (!camera.contains(soa.position[i]))
            continue;
        const auto& visual = presets.getShell(soa.preset[i]).visual;
        renderer.draw(rect, soa.position[i], visual.size + OFFSET * 2.f, visual.origin + OFFSET, t1::PI - soa.angle[i], cl::ORANGE);
    }
}
