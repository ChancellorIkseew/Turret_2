#include "turrets_system.hpp"
//
#include "mob_manager.hpp"
#include "shell_manager.hpp"

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

static inline void shoot(MobSoA& soa, ShellManager& shells, const size_t mobCount) {
    for (size_t i = 0; i < mobCount; ++i) {
        if (soa.restReloadTime[i] > 0)
            continue;
        soa.restReloadTime[i] = soa.preset[i]->turret->reload;
        const auto& preset = soa.preset[i]->turret->shell;
        PixelCoord position = soa.position[i];
        const AngleRad angle = soa.angle[i];


        position = position + soa.preset[i]->turret->barrels[0];
        shells.addShell(preset, position, angle, preset->damage, preset->maxLifeTime, soa.teamID[i]);
    }
}

void turrets::processTurrets(MobSoA& soa, ShellManager& shells) {
    const size_t mobCount = soa.mobCount;
    reduceRestReload(soa);
    shoot(soa, shells, mobCount);
}

void turrets::drawTurrets(const MobSoA& soa, const Camera& camera, const float tickOfset) {
    /*Sprite sprite;
    const size_t mobCount = soa.mobCount;
    for (size_t i = 0; i < mobCount; ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            continue;
        auto& visual = soa.preset[i]->visual;
        sprite.setTexture(*visual.texture);
        sprite.setOrigin(visual.origin);
        sprite.setSize(visual.size);
        sprite.setPosition(soa.position[i] + soa.velocity[i] * tickOfset);
        sprite.setRotationRad(soa.angle[i]);
        sprite.draw();
    }*/
}
