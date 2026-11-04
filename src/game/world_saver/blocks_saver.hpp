#pragma once
// blocks_saver.hpp — save/load BlockMap (polymorphic blocks)
//
// Format (binary, packed via archive::pack):
//   [size_t blockCount]              — number of master blocks
//   [BlockRecord × blockCount]       — one per master block (LinkBlocks NOT saved)
//
// BlockRecord:
//   [BlockType type]         1 byte   — dispatch tag (in_progress ≠ preset.archetype)
//   [TileCoord tile]         8 bytes  — master tile position
//   [TeamID teamID]          1 byte   — from BlockTile::teamID
//   [BlockPresetID presetID] 1 byte   — for makeBlock reconstruction
//   [Health health]          8 bytes  — current damage (overrides preset.maxHealth)
//   [BlockRot rotation]      1 byte   — from block->getRotation()
//   [subclass state]         variable — dispatched by BlockType
//
// NOT saved (derived from preset via makeBlock):
//   - Block::size, Block::textureRect
//   - TurretBlock::turretPreset (from preset.turret)
//   - DrillBlock::productionTime (from preset.productionTime)
//   - FactoryBlock::productionTime (from preset in ctor)
//   - CoreBlock::team pointer (reconstructed via syncTeam on first tick)
//
// NOT saved (reconstructed):
//   - LinkBlock — reconstructed via place() during load
//   - TurretSoA — saved/loaded separately via turrets_saver (turrets.dat)
//
// Load order: blocks BEFORE turrets.
//   place() creates temporary turret SoA entries → loadTurrets() overwrites entire SoA.
//
// Future preset verification: presets reorder to match manifest BEFORE world data loads.
// Saved presetIDs match directly — no remap needed in this saver.

#include "engine/io/archive.hpp"
#include "engine/io/io.hpp"
#include "game/blocks/block_map.hpp"
#include "game/blocks/block_types.hpp"
#include "game/blocks/make_block.hpp"

class Presets;

// === Save ===

inline void saveBlockState(const Block& block, std::string& blob) {
    switch (block.getType()) {
    case BlockType::wall:
        break;
    case BlockType::belt: {
        const auto& b = static_cast<const BeltBlock&>(block);
        archive::add(blob, b.itemID);              // ItemPresetID[3] — trivially copyable array
        archive::add(blob, b.itemY);               // float[3]
        archive::add(blob, b.itemX);               // float[3]
        archive::add(blob, b.minItem);
        archive::add(blob, b.len);
        break;
    }
    case BlockType::drill: {
        const auto& b = static_cast<const DrillBlock&>(block);
        archive::add(blob, b.inventory);
        archive::add(blob, b.restProductionTime);
        archive::add(blob, b.step);
        break;
    }
    case BlockType::impact_drill: {
        const auto& b = static_cast<const ImpactDrillBlock&>(block);
        archive::add(blob, b.inventory);
        archive::add(blob, b.restProductionTime);
        archive::add(blob, b.step);
        break;
    }
    case BlockType::factory: {
        const auto& b = static_cast<const FactoryBlock&>(block);
        archive::add(blob, b.inputs);              // InplaceVector<ItemStack, 4> — trivially copyable
        archive::add(blob, b.output);              // ItemStack — trivially copyable
        archive::add(blob, b.restProductionTime);
        archive::add(blob, b.step);
        break;
    }
    case BlockType::turret: {
        const auto& b = static_cast<const TurretBlock&>(block);
        archive::add(blob, b.ammo);                // ItemStack — only block-level state
        break;
    }
    case BlockType::junction: {
        const auto& b = static_cast<const JunctionBlock&>(block);
        archive::add(blob, b.vertical);            // RotatedItem — trivially copyable
        archive::add(blob, b.horizontal);
        break;
    }
    case BlockType::router: {
        const auto& b = static_cast<const RouterBlock&>(block);
        archive::add(blob, b.inventory);
        archive::add(blob, b.step);
        break;
    }
    case BlockType::core: {
        const auto& b = static_cast<const CoreBlock&>(block);
        archive::add(blob, b.preveouseTickHealth);
        break;
    }
    case BlockType::in_progress: {
        const auto& b = static_cast<const InProgress&>(block);
        archive::add(blob, b.action);
        archive::add(blob, b.progress);
        // rotation is in base fields (via getRotation() override)
        break;
    }
    default:
        break;
    }
}

inline void saveBlocks(const BlockMap& blocks, const std::filesystem::path& path) {
    std::string blob;
    const TileCoord size = blocks.getSize();
    // Count master blocks (skip air + link)
    size_t blockCount = 0;
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            const BlockType type = blocks.at(x, y).type;
            if (type != BlockType::air && type != BlockType::link)
                ++blockCount;
        }
    }
    archive::add(blob, blockCount);
    // Save each master block
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            const BlockTile& tile = blocks.at(x, y);
            if (tile.type == BlockType::air || tile.type == BlockType::link)
                continue;
            const Block& block = *tile.block;
            const TileCoord tileCoord(x, y);
            // Base fields
            archive::add(blob, block.getType());
            archive::add(blob, tileCoord);
            archive::add(blob, tile.teamID);
            archive::add(blob, block.presetID);
            archive::add(blob, block.health);
            archive::add(blob, block.getRotation());
            // Subclass state
            saveBlockState(block, blob);
        }
    }
    const std::string packedData = archive::pack(blob);
    io::writeFile(path / "blocks.dat", packedData);
}

inline void loadBlockState(Block& block, archive::Reader& reader) {
    switch (block.getType()) {
    case BlockType::wall:
        break;
    case BlockType::belt: {
        auto& b = static_cast<BeltBlock&>(block);
        reader.get(b.itemID);
        reader.get(b.itemY);
        reader.get(b.itemX);
        reader.get(b.minItem);
        reader.get(b.len);
        break;
    }
    case BlockType::drill: {
        auto& b = static_cast<DrillBlock&>(block);
        reader.get(b.inventory);
        reader.get(b.restProductionTime);
        reader.get(b.step);
        break;
    }
    case BlockType::impact_drill: {
        auto& b = static_cast<ImpactDrillBlock&>(block);
        reader.get(b.inventory);
        reader.get(b.restProductionTime);
        reader.get(b.step);
        break;
    }
    case BlockType::factory: {
        auto& b = static_cast<FactoryBlock&>(block);
        reader.get(b.inputs);
        reader.get(b.output);
        reader.get(b.restProductionTime);
        reader.get(b.step);
        break;
    }
    case BlockType::turret: {
        auto& b = static_cast<TurretBlock&>(block);
        reader.get(b.ammo);
        break;
    }
    case BlockType::junction: {
        auto& b = static_cast<JunctionBlock&>(block);
        reader.get(b.vertical);
        reader.get(b.horizontal);
        break;
    }
    case BlockType::router: {
        auto& b = static_cast<RouterBlock&>(block);
        reader.get(b.inventory);
        reader.get(b.step);
        break;
    }
    case BlockType::core: {
        auto& b = static_cast<CoreBlock&>(block);
        reader.get(b.preveouseTickHealth);
        break;
    }
    case BlockType::in_progress:
        // InProgress state (action, progress) is read in loadBlocksInto directly
        break;
    default:
        break;
    }
}

// Loads blocks INTO an existing BlockMap (which was initialized with mapSize).
// Needs Presets for makeBlock reconstruction.
// CRITICAL: must be called BEFORE loadTurrets — place() creates temporary
// turret SoA entries that get overwritten by turrets.dat.
inline void loadBlocks(BlockMap& blocks, const Presets& presets, const std::filesystem::path& path) {
    const std::string packedData = io::readFile(path / "blocks.dat");
    const std::string blob = archive::unpack(packedData);
    archive::Reader reader(blob);
    size_t blockCount;
    reader.get(blockCount);
    for (size_t i = 0; i < blockCount; ++i) {
        // Read base fields
        BlockType type;
        TileCoord tileCoord;
        TeamID teamID;
        BlockPresetID presetID;
        Health health;
        BlockRot rotation;
        reader.get(type);
        reader.get(tileCoord);
        reader.get(teamID);
        reader.get(presetID);
        reader.get(health);
        reader.get(rotation);

        std::unique_ptr<Block> block;
        if (type == BlockType::in_progress) {
            // InProgress is NOT created via makeBlock (makeBlock uses preset.archetype,
            // which for InProgress points to the FINAL block, not in_progress)
            block = std::make_unique<InProgress>();
            auto* ip = static_cast<InProgress*>(block.get());
            reader.get(ip->action);
            reader.get(ip->progress);
            ip->rotation = rotation;
            // Set base fields from preset (size, textureRect)
            const BlockPreset& preset = presets.getBlock(presetID);
            block->presetID = presetID;
            block->size = preset.size;
            block->textureRect = preset.visual.textureRect;
            block->health = health;
        } else {
            // Normal blocks via makeBlock
            block = makeBlock(presets, presetID, rotation);
            // Override health with saved value (makeBlock sets preset.maxHealth)
            block->health = health;
            // Load subclass-specific runtime state
            loadBlockState(*block, reader);
        }

        // Place block — creates LinkBlocks + registers in meta (cores, in-progress, turrets)
        // Turret SoA entries from place() will be overwritten by loadTurrets() later
        blocks.place(tileCoord, teamID, block, rotation);
    }
}
