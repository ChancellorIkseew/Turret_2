#pragma once
#include <array>
#include <cassert>
#include <string>
#include <unordered_map>
#include "preset_defs.hpp"

class Atlas;
constexpr size_t MAX_PRESETS = 64;

class Presets {
    std::array<BlockPreset, MAX_PRESETS>   blockStore;
    std::array<ItemPreset, MAX_PRESETS>     itemStore;
    std::array<MobPreset, MAX_PRESETS>       mobStore;
    std::array<OrePreset, MAX_PRESETS>       oreStore;
    std::array<ShellPreset, MAX_PRESETS>   shellStore;
    std::array<TurretPreset, MAX_PRESETS> turretStore;

    std::unordered_map<std::string, BlockPresetID>  blockIDByName;
    std::unordered_map<std::string, ItemPresetID>   itemIDByName;
    std::unordered_map<std::string, MobPresetID>    mobIDByName;
    std::unordered_map<std::string, OrePresetID>    oreIDByName;
    std::unordered_map<std::string, ShellPresetID>  shellIDByName;
    std::unordered_map<std::string, TurretPresetID> turretIDByName;

    BlockPresetID  nextBlockID  = BlockPresetID(2); // air, auxilary
    ItemPresetID   nextItemID   = ItemPresetID(0);
    MobPresetID    nextMobID    = MobPresetID(0);
    OrePresetID    nextOreID    = OrePresetID(1); // air
    ShellPresetID  nextShellID  = ShellPresetID(0);
    TurretPresetID nextTurretID = TurretPresetID(0);
public:
    void load(const Atlas& atlas);

    bool hasBlockID(const std::string& name) const { return blockIDByName.contains(name); }
    bool hasitemID(const std::string& name) const { return itemIDByName.contains(name); }
    bool hasMobID(const std::string& name) const { return mobIDByName.contains(name); }
    bool hasOreID(const std::string& name) const { return oreIDByName.contains(name); }
    bool hasShellID(const std::string& name) const { return shellIDByName.contains(name); }
    bool hasTurretID(const std::string& name) const { return turretIDByName.contains(name); }

    BlockPresetID getBlockID(const std::string& name) const { return blockIDByName.at(name); }
    ItemPresetID getItemID(const std::string& name) const { return itemIDByName.at(name); }
    MobPresetID getMobID(const std::string& name) const { return mobIDByName.at(name); }
    OrePresetID getOreID(const std::string& name) const { return oreIDByName.at(name); }
    ShellPresetID getShellID(const std::string& name) const { return shellIDByName.at(name); }
    TurretPresetID getTurretID(const std::string& name) const { return turretIDByName.at(name); }

    const BlockPreset& getBlock(BlockPresetID id) const noexcept {
        return blockStore[id.asUint()];
    }
    const ItemPreset& getItem(ItemPresetID id) const noexcept {
        return itemStore[id.asUint()];
    }
    const MobPreset& getMob(MobPresetID id) const noexcept {
        return mobStore[id.asUint()];
    }
    const OrePreset& getOre(OrePresetID id) const noexcept {
        return oreStore[id.asUint()];
    }
    const ShellPreset& getShell(ShellPresetID id) const noexcept {
        return shellStore[id.asUint()];
    }
    const TurretPreset& getTurret(TurretPresetID id) const noexcept {
        return turretStore[id.asUint()];
    }
    const auto& getOres()   const { return oreIDByName; }
    const auto& getBlocks() const { return blockIDByName; }
private:
    template<class PresetType>
    void loadPresets(const std::string& folder, const Atlas& atlas);
};
