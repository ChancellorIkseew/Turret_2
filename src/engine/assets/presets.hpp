#pragma once
#include <cassert>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "game/physics/mob_manager.hpp"

using PresetID = uint8_t;
class Atlas;
constexpr size_t MAX_PRESETS = 64;

class Presets {
    std::array<MobPreset, MAX_PRESETS>       mobStore;
    std::array<ShellPreset, MAX_PRESETS>   shellStore;
    std::array<TurretPreset, MAX_PRESETS> turretStore;

    std::unordered_map<std::string, PresetID>    mobIDByName;
    std::unordered_map<std::string, PresetID>  shellIDByName;
    std::unordered_map<std::string, PresetID> turretIDByName;

    PresetID    nextMobID = 0;
    PresetID  nextShellID = 0;
    PresetID nextTurretID = 0;
public:
    const MobPreset& getMob(PresetID id) const noexcept {
        return mobStore[id];
    }
    const ShellPreset& getShell(PresetID id) const noexcept {
        return shellStore[id];
    }
    const TurretPreset& getTurret(PresetID id) const noexcept {
        return turretStore[id];
    }

    void load(const Atlas& atlas);

private:
    template<class PresetType>
    void loadPresets(const std::string& folder, const Atlas& atlas);
};
