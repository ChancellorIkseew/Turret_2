#pragma once
#include <array>
#include <cassert>
#include <string>
#include <unordered_map>
#include "preset_defs.hpp"

class Atlas;
constexpr size_t MAX_PRESETS = 64;

class Presets {
    std::array<MobPreset, MAX_PRESETS>       mobStore;
    std::array<ShellPreset, MAX_PRESETS>   shellStore;
    std::array<TurretPreset, MAX_PRESETS> turretStore;

    std::unordered_map<std::string, MobPresetID>    mobIDByName;
    std::unordered_map<std::string, ShellPresetID>  shellIDByName;
    std::unordered_map<std::string, TurretPresetID> turretIDByName;

    MobPresetID    nextMobID    = MobPresetID(0);
    ShellPresetID  nextShellID  = ShellPresetID(0);
    TurretPresetID nextTurretID = TurretPresetID(0);
public:
    void load(const Atlas& atlas);

    bool hasMobID(const std::string& name) const { return mobIDByName.contains(name); }
    bool hasShellID(const std::string& name) const { return shellIDByName.contains(name); }
    bool hasTurretID(const std::string& name) const { return turretIDByName.contains(name); }

    MobPresetID getMobID(const std::string& name) const { return mobIDByName.at(name); }
    ShellPresetID getShellID(const std::string& name) const { return shellIDByName.at(name); }
    TurretPresetID getTurretID(const std::string& name) const { return turretIDByName.at(name); }

    const MobPreset& getMob(MobPresetID id) const noexcept {
        return mobStore[id.asUint()];
    }
    const ShellPreset& getShell(ShellPresetID id) const noexcept {
        return shellStore[id.asUint()];
    }
    const TurretPreset& getTurret(TurretPresetID id) const noexcept {
        return turretStore[id.asUint()];
    }
private:
    template<class PresetType>
    void loadPresets(const std::string& folder, const Atlas& atlas);
};
