#include "turrets_system.hpp"
//
#include "game/player/camera.hpp"
#include "mob_manager.hpp"
#include "shell_manager.hpp"
#include "engine/audio/audio.hpp"

using ItemType = uint16_t;
struct ItemStack {
    ItemType itemType;
    uint64_t count;
};

static inline void reduceRestReload(MobSoA& soa) {
    for (auto& time : soa.restReloadTime) {
        if (time > 0)
            --time;
    }
}

static inline void shoot(MobSoA& soa, ShellManager& shells, const size_t mobCount, const Camera& camera, Audio& audioManager) {
    for (size_t i = 0; i < mobCount; ++i) {
        if (soa.restReloadTime[i] > 0 || !soa.shootingData[i].isShooting)
            continue;
        soa.restReloadTime[i] = soa.preset[i]->turret->reload;
        const auto& preset = soa.preset[i]->turret->shell;
        const AngleRad angle = soa.turretAngle[i];
        const PixelCoord localMuzzle = soa.preset[i]->turret->barrels[soa.currentBarrel[i]];
        PixelCoord position = soa.position[i];

        ++soa.currentBarrel[i];
        if (soa.currentBarrel[i] >= soa.preset[i]->turret->barrelsCount)
            soa.currentBarrel[i] = 0;

        const float sin = sinf(angle);
        const float cos = cosf(angle);
        position.x +=  localMuzzle.x * cos + localMuzzle.y * sin;
        position.y += -localMuzzle.x * sin + localMuzzle.y * cos;
        shells.addShell(preset, position, angle, preset->damage, preset->maxLifeTime, soa.teamID[i]);
        if (camera.contains(t1::tile(position)))
            audioManager.playDiegetic("cannon_shot", position, camera);
    }
}

void turrets::processTurrets(MobSoA& soa, ShellManager& shells, const Camera& camera, Audio& audioManager) {
    const size_t mobCount = soa.mobCount;
    reduceRestReload(soa);
    shoot(soa, shells, mobCount, camera, audioManager);
}

void turrets::drawTurrets(const MobSoA& soa, const Camera& camera) {
    Sprite sprite;
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        auto& visual = soa.preset[i]->turret->visual;
        sprite.setTexture(*visual.texture);
        sprite.setOrigin(visual.origin);
        sprite.setSize(visual.size);
        sprite.setPosition(soa.position[i]);
        sprite.setRotationRad(soa.turretAngle[i]);
        sprite.draw();
    }
}
