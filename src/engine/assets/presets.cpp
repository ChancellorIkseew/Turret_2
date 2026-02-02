#include "presets.hpp"
//
#include "engine/io/folders.hpp"
#include "game/physics/mob_manager.hpp"
#include "game/physics/shell_manager.hpp"
#include "preset_reader.hpp"

template<class Tag>
using FindMap = std::unordered_map<std::string, preset_tag::StrongID<Tag>>;
using ShellFindMap = FindMap<preset_tag::ShellTag>;
using TurretFindMap = FindMap<preset_tag::TurretTag>;

static debug::Logger logger("presets");

static auto createBlockPreset(const PresetReader& reader, const Atlas& atlas) {
    std::array<uint8_t, 16> frames;
    size_t frameCount = reader.getArray<uint8_t>("frame_order", frames);
    BlockVisualPreset visual{
        reader.getTexture(atlas, "texture"),
        reader.get<PixelCoord>("origin"),
        reader.get<PixelCoord>("size"),
        reader.get<uint8_t>("frame_ticks"),
        reader.get<float>("frame_height"),
        static_cast<uint8_t>(frameCount),
        frames
    };
    return BlockPreset{
        reader.get<Health>("health"),
        visual
    };
}

static auto createMobPreset(const PresetReader& reader, const Atlas& atlas, const TurretFindMap& turretIDByName) {
    std::array<uint8_t, 16> frames;
    size_t frameCount = reader.getArray<uint8_t>("frame_order", frames);
    MobVisualPreset visual{
        reader.getTexture(atlas, "texture"),
        reader.get<PixelCoord>("origin"),
        reader.get<PixelCoord>("size"),
        reader.get<uint8_t>("frame_ticks"),
        reader.get<float>("frame_height"),
        static_cast<uint8_t>(frameCount),
        frames
    };
    return MobPreset{
        reader.get<float>("speed"),
        reader.get<float>("hitbox_radius"),
        reader.get<Health>("health"),
        mob_ai::getMovingAI(reader.get<std::string>("moving_ai")),
        mob_ai::getShootingAI(reader.get<std::string>("moving_ai")),
        reader.getID(turretIDByName, "turret"),
        visual
    };
}

static auto createShellPreset(const PresetReader& reader, const Atlas& atlas) {
    ShellVisualPreset visual{
        reader.getTexture(atlas, "texture"),
        reader.get<PixelCoord>("origin"),
        reader.get<PixelCoord>("size")
    };
    Explosion explosion{
        reader.get<Health>("explosion_damage"),
        reader.get<int>("explosion_radius"),
        reader.get<float>("explosion_fading")
    };
    return ShellPreset{
        reader.get<float>("speed"),
        reader.get<Health>("damage"),
        reader.get<TickCount>("life_time"),
        explosion,
        visual
    };
}

static auto createTurretPreset(const PresetReader& reader, const Atlas& atlas, const ShellFindMap& shellIDByName) {
    std::array<PixelCoord, 4> barrels;
    size_t barrelsCount = reader.getArray<PixelCoord>("barrels", barrels);
    TurretVisualPreset visual{
        reader.getTexture(atlas, "texture"),
        reader.get<PixelCoord>("origin"),
        reader.get<PixelCoord>("size"),
        reader.get<uint8_t>("frame_count")
    };
    return TurretPreset{
        reader.get<TickCount>("reload"),
        reader.get<float>("range"),
        reader.get<AngleRad>("rotation_speed"),
        static_cast<uint8_t>(barrelsCount),
        barrels,
        reader.getID(shellIDByName, "shell"),
        visual
    };
}

template<class PresetType, class Tag>
static inline void addPreset(const std::string& name, PresetType&& preset,
    std::array<PresetType, MAX_PRESETS>& store, FindMap<Tag>& find, preset_tag::StrongID<Tag>& nextID) {
    preset_tag::StrongID<Tag> id;
    if (find.contains(name)) id = find[name];
    else                     id = nextID++;
    find.insert_or_assign(name, id);
    store[id.asUint()] = std::move(preset);
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
            if constexpr (std::is_same_v<PresetType, BlockPreset>)
                addPreset(name, createBlockPreset(reader, atlas), blockStore, blockIDByName, nextBlockID);
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
    loadPresets<BlockPreset>("blocks", atlas);
}
