#include "presets.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/render/atlas.hpp"
#include "game/physics/mob_manager.hpp"
#include "game/physics/shell_manager.hpp"
#include "preset_reader.hpp"

using FindMap = std::unordered_map<std::string, PresetID>;
static debug::Logger logger("presets");

static auto createMobPreset(const PresetReader& reader, const Atlas& atlas, const FindMap& turretIDByName) {
    std::array<uint8_t, 16> frames;
    reader.getUint8Array("frame_order", frames);
    MobVisualPreset visual(
        reader.getTexture(atlas, "texture"),
        reader.get(&tin::Data::getPixelCoord, "origin"),
        reader.get(&tin::Data::getPixelCoord, "size"),
        reader.get(&tin::Data::getUint8, "frame_ticks"),
        reader.get(&tin::Data::getFloat, "frame_height"),
        reader.get(&tin::Data::getUint8, "frame_count"),
        frames
    );
    return MobPreset(
        reader.get(&tin::Data::getFloat, "speed"),
        reader.get(&tin::Data::getFloat, "hitbox_radius"),
        reader.get(&tin::Data::getInt64, "health"),
        mob_ai::getMovingAI(reader.getString("moving_ai")),
        mob_ai::getShootingAI(reader.getString("shooting_ai")),
        reader.getID(turretIDByName, "turret"),
        visual
    );
}

static auto createShellPreset(const PresetReader& reader, const Atlas& atlas) {
    ShellVisualPreset visual(
        reader.getTexture(atlas, "texture"),
        reader.get(&tin::Data::getPixelCoord, "origin"),
        reader.get(&tin::Data::getPixelCoord, "size")
    );
    Explosion explosion(
        reader.get(&tin::Data::getInt64, "explosion_damage"),
        reader.get(&tin::Data::getInt32, "explosion_radius"),
        reader.get(&tin::Data::getFloat, "explosion_fading")
    );
    return ShellPreset(
        reader.get(&tin::Data::getFloat, "speed"),
        reader.get(&tin::Data::getInt64, "damage"),
        reader.get(&tin::Data::getUint16, "life_time"),
        explosion,
        visual
    );
}

static auto createTurretPreset(const PresetReader& reader, const Atlas& atlas, const FindMap& shellIDByName) {
    std::array<PixelCoord, 4> barrels;
    size_t barrelsCount = reader.getPixelCoordArray("barrels", barrels);
    TurretVisualPreset visual(
        reader.getTexture(atlas, "texture"),
        reader.get(&tin::Data::getPixelCoord, "origin"),
        reader.get(&tin::Data::getPixelCoord, "size"),
        reader.get(&tin::Data::getUint8, "frame_count")
    );
    return TurretPreset(
        reader.get(&tin::Data::getUint16, "reload"),
        reader.get(&tin::Data::getFloat, "rotation_speed"),
        barrelsCount,
        barrels,
        reader.getID(shellIDByName, "shell"),
        visual
    );
}

template<class PresetType>
static inline void addPreset(const std::string& name, PresetType&& preset,
    std::array<PresetType, MAX_PRESETS>& store, FindMap& find, PresetID& nextID) {
    PresetID id;
    if (find.contains(name)) id = find[name];
    else                     id = nextID++;
    find.insert_or_assign(name, id);
    store[id] = std::move(preset);
}

template<class PresetType>
void Presets::loadPresets(const std::string& folder, const Atlas& atlas) {
    const auto path = io::folders::CONTENT / folder;
    const auto contents = io::folders::getContents(path, io::folders::ContentsType::file);
    for (const auto& file : contents) {
        const auto data = tin::read(path / file, tin::Log::only_error);
        bool failed = false;
        const PresetReader reader(data, file, failed);
        try {
            const std::string name = io::folders::trimExtensions(file);
            if constexpr (std::is_same_v<PresetType, MobPreset>)
                addPreset(name, createMobPreset(reader, atlas, turretIDByName), mobStore, mobIDByName, nextMobID);
            if constexpr (std::is_same_v<PresetType, ShellPreset>)
                addPreset(name, createShellPreset(reader, atlas), shellStore, shellIDByName, nextShellID);
            if constexpr (std::is_same_v<PresetType, TurretPreset>)
                addPreset(name, createTurretPreset(reader, atlas, shellIDByName), turretStore, turretIDByName, nextTurretID);
            logger.debug() << "Preset created: " << name;
        }
        catch (const std::bad_optional_access&) {
            logger.error() << "Failed to create preset (missing data in file): " << file;
        }
        catch (const std::out_of_range&) {
            logger.error() << "Failed to create preset (dependency ID not found): " << file;
        }
    }
}

void Presets::load(const Atlas& atlas) {
    loadPresets<ShellPreset>("shells", atlas);
    loadPresets<TurretPreset>("turrets", atlas);
    loadPresets<MobPreset>("mobs", atlas);
}
