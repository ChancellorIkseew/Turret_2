#include "shells_system.hpp"
//
#include <cmath>
#include "engine/assets/presets.hpp"
#include "engine/render/renderer.hpp"
#include "game/player/camera.hpp"
#include "game/physics/chunk_grid.hpp"
#include "game/physics/mob_manager.hpp"
#include "game/physics/shell_manager.hpp"

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
            const Health takenDamage = std::min(mobs.health[mob], shells.restDamage[shell]);
            shells.restDamage[shell] -= takenDamage;
            mobs.health[mob] -= takenDamage;
            if (shells.restDamage[shell] < 1)
                break;
        }
    }
}

void shells::processShells(ShellSoA& shells, MobSoA& mobs, const ChunkGrid& chunks) {
    const size_t shellCount = shells.shellCount;
    reduceShellsLifeTime(shells);
    moveShells(shells, shellCount);
    hitMobs(shells, mobs, shellCount, chunks);
}

void shells::cleanupShells(ShellManager& manager, const Presets& presets/*, Explosions& explosions*/) {
    const auto& soa = manager.getSoa();
    // Reverse itaretion to avoid bugs with "swap and pop".
    for (size_t i = soa.shellCount; i > 0; --i) {
        size_t index = i - 1;
        if (soa.restLifeTime[index] > 0 && soa.restDamage[index] > 0)
            continue;
        // if (soa.presets->explosion.damage != 0)
        //     explosions.push(soa.presets->explosion);
        manager.removeShell(index);
    }
}

void shells::drawShells(const ShellSoA& soa, const Presets& presets, const Camera& camera, const Renderer& renderer) {
    const size_t shellCount = soa.shellCount;
    for (size_t i = 0; i < shellCount; ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        auto& visual = presets.getShell(soa.preset[i]).visual;
        renderer.draw(visual.texture, soa.position[i], visual.size, visual.origin, soa.angle[i]);
    }
}
