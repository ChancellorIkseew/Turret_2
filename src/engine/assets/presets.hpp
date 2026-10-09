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

    std::unordered_map<std::string, BlockPresetID>  blockFindMap;
    std::unordered_map<std::string, ItemPresetID>   itemFindMap;
    std::unordered_map<std::string, MobPresetID>    mobFindMap;
    std::unordered_map<std::string, OrePresetID>    oreFindMap;
    std::unordered_map<std::string, ShellPresetID>  shellFindMap;
    std::unordered_map<std::string, TurretPresetID> turretFindMap;

    BlockPresetID  nextBlockID  = BlockPresetID(1); // air
    ItemPresetID   nextItemID   = ItemPresetID(1); // air(error)
    MobPresetID    nextMobID    = MobPresetID(0);
    OrePresetID    nextOreID    = OrePresetID(1); // air
    ShellPresetID  nextShellID  = ShellPresetID(0);
    TurretPresetID nextTurretID = TurretPresetID(0);
public:
    void load(const Atlas& atlas);

    bool hasBlockID(const std::string& name) const { return blockFindMap.contains(name); }
    bool hasItemID(const std::string& name) const { return itemFindMap.contains(name); }
    bool hasMobID(const std::string& name) const { return mobFindMap.contains(name); }
    bool hasOreID(const std::string& name) const { return oreFindMap.contains(name); }
    bool hasShellID(const std::string& name) const { return shellFindMap.contains(name); }
    bool hasTurretID(const std::string& name) const { return turretFindMap.contains(name); }

    BlockPresetID getBlockID(const std::string& name) const { return blockFindMap.at(name); }
    ItemPresetID getItemID(const std::string& name) const { return itemFindMap.at(name); }
    MobPresetID getMobID(const std::string& name) const { return mobFindMap.at(name); }
    OrePresetID getOreID(const std::string& name) const { return oreFindMap.at(name); }
    ShellPresetID getShellID(const std::string& name) const { return shellFindMap.at(name); }
    TurretPresetID getTurretID(const std::string& name) const { return turretFindMap.at(name); }

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
    const auto& getOres()   const { return oreFindMap; }
    const auto& getBlocks() const { return blockFindMap; }
    const auto& getItems()  const { return itemFindMap; }
private:
    template<class PresetType>
    void loadPresets(const std::string& folder, const Atlas& atlas);
};
