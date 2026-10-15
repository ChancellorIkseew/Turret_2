#include "presets.hpp"
//
#include "engine/io/folders.hpp"
#include "preset_reader.hpp"

template<class Tag>
using FindMap = std::unordered_map<std::string, preset_tag::StrongID<Tag>>;
using ItemFindMap = FindMap<preset_tag::ItemTag>;
using ShellFindMap = FindMap<preset_tag::ShellTag>;
using TurretFindMap = FindMap<preset_tag::TurretTag>;

static debug::Logger logger("presets");

static BlockType getBlockType(const std::string name) {
    if (name == "wall")    return BlockType::wall;
    if (name == "drill")   return BlockType::drill;
    if (name == "impact_drill") return BlockType::impact_drill;
    if (name == "belt")    return BlockType::belt;
    if (name == "bridge")  return BlockType::bridge;
    if (name == "factory") return BlockType::factory;
    if (name == "intersection") return BlockType::junction;
    if (name == "router")  return BlockType::router;
    if (name == "turret")  return BlockType::turret;
    if (name == "core")    return BlockType::core;
    return BlockType::air;
}

static std::optional<Ingredient> parseIng(const std::string& text, const FindMap<preset_tag::ItemTag>& itemFindMap) {
    const size_t bar = text.find('|');
    if (bar == std::string::npos)
        return std::nullopt;
    const std::string name = text.substr(0, bar);
    const auto count = validator::to<decltype(Ingredient::amount)>(text.substr(bar + 1));
    if (!count || *count < 1 || !itemFindMap.contains(name))
        return std::nullopt;
    return Ingredient(itemFindMap.at(name), *count);
}

template<size_t CAPACITY>
static auto loadIngridients(const PresetReader& reader, const FindMap<preset_tag::ItemTag>& itemFindMap, const std::string& key) {
    InplaceVector<Ingredient, CAPACITY> recipe;
    for (auto& strItemStack : reader.getList(key)) {
         recipe.push_back(*parseIng(strItemStack, itemFindMap));
    }
    if (recipe.empty())
        throw std::bad_optional_access();
    return recipe;
}

static auto createBlockPreset(const PresetReader& reader, const Atlas& atlas, const TurretFindMap& turretFindMap,
    const ItemFindMap& itemFindMap, const std::string& name) {
    std::array<uint8_t, 16> frames;
    BlockVisualPreset visual{
        reader.getTexture(atlas, "texture"),
        reader.get<uint8_t>("frame_ticks"),
        reader.get<float>("frame_height") / atlas.getSize().y,
        reader.getArray<uint8_t>("frame_order", frames),
        frames
    };
    BlockPreset preset;
    preset.archetype = getBlockType(reader.get<std::string>("archetype"));
    preset.size = reader.get<uint8_t>("size");
    preset.buildTime = reader.get<int16_t>("build_time");
    preset.rotatable = reader.get<bool>("rotatable");
    preset.maxHealth = reader.get<Health>("health");
    preset.visual = visual;
    preset.recipe = loadIngridients<preset.recipe.capacity()>(reader, itemFindMap, "recipe");
    if (preset.archetype == BlockType::turret)
        preset.turret = reader.getID(turretFindMap, "turret");
    if (preset.archetype == BlockType::factory) {
        preset.inputItems = loadIngridients<preset.inputItems.capacity()>(reader, itemFindMap, "input_items");
        preset.outputItem = *parseIng(reader.get<std::string>("output_item"), itemFindMap);
    }
    if (preset.archetype == BlockType::factory || preset.archetype == BlockType::drill)
        preset.productionTime = reader.get<int16_t>("production_time");
    return preset;
}

static auto createItemPreset(const PresetReader& reader, const Atlas& atlas) {
    return ItemPreset{
        reader.getTexture(atlas, "texture"),
    };
}

static auto createOrePreset(const PresetReader& reader, const Atlas& atlas, const ItemFindMap& itemFindMap) {
    return OrePreset{
        reader.getID(itemFindMap, "item"),
        reader.getTexture(atlas, "texture")
    };
}

static auto createMobPreset(const PresetReader& reader, const Atlas& atlas, const TurretFindMap& turretFindMap) {
    std::array<uint8_t, 16> frameOrder;
    std::array<PixelCoord, 4> engines;
    MobVisualPreset visual{
        reader.getTexture(atlas, "texture"),
        reader.get<PixelCoord>("origin"),
        reader.get<PixelCoord>("size"),
        reader.get<uint8_t>("frame_ticks"),
        reader.get<float>("frame_height") / atlas.getSize().y,
        reader.getArray<uint8_t>("frame_order", frameOrder),
        frameOrder,
        reader.getOptArray<PixelCoord>("engines", engines),
        engines
    };
    return MobPreset{
        reader.get<float>("speed"),
        reader.get<float>("hitbox_radius"),
        reader.get<float>("shield_radius"),
        reader.get<Health>("health"),
        reader.get<Health>("shield_health"),
        reader.get<bool>("flying"),
        reader.get<uint8_t>("build_speed"),
        mob_ai::getMovingAI(reader.get<std::string>("moving_ai")),
        mob_ai::getShootingAI(reader.get<std::string>("shooting_ai")),
        reader.getID(turretFindMap, "turret"),
        visual
    };
}

static auto createShellPreset(const PresetReader& reader, const Atlas& atlas) {
    ShellVisualPreset visual{
        reader.getTexture(atlas, "texture"),
        reader.get<PixelCoord>("origin"),
        reader.get<PixelCoord>("size"),
        reader.get<bool>("has_trail")
    };
    Explosion explosion{
        reader.get<Health>("explosion_damage"),
        reader.get<float>("explosion_radius"),
        reader.get<int8_t>("shards_count")
    };
    return ShellPreset{
        reader.get<float>("speed"),
        reader.get<Health>("damage"),
        reader.get<TickCount>("life_time"),
        explosion,
        visual
    };
}

static auto createTurretPreset(const PresetReader& reader, const Atlas& atlas, const ShellFindMap& shellFindMap,
    const ItemFindMap& itemIDByName) {
    std::array<PixelCoord, 4> barrels;
    std::array<PixelCoord, 4> ejectionPorts;
    std::array<PixelCoord, 4> sparkAreas;
    TurretVisualPreset visual{
        reader.getTexture(atlas, "texture"),
        reader.get<PixelCoord>("origin"),
        reader.get<PixelCoord>("size"),
        reader.get<float>("shadow_offset"),
        reader.get<std::string>("shot_sound"),
        reader.getOptArray<PixelCoord>("ejection_ports", ejectionPorts),
        reader.getOptArray<PixelCoord>("spark_areas", sparkAreas),
        reader.getOpt<PixelCoord>("spark_area_size"),
        ejectionPorts,
        sparkAreas
    };
    constexpr float RAD_TO_DEGREE = t1::PI / 180.f;
    return TurretPreset{
        reader.get<TickCount>("reload"),
        reader.get<float>("range"),
        reader.get<float>("recoil"),
        reader.get<AngleRad>("spread_deg") * RAD_TO_DEGREE,
        reader.get<AngleRad>("rotation_speed"),
        itemIDByName.at(reader.get<std::string>("ammo")),
        reader.get<uint8_t>("ammo_by_item"),
        reader.getArray<PixelCoord>("barrels", barrels),
        barrels,
        reader.getID(shellFindMap, "shell"),
        visual
    };
}

template<class PresetType, class Tag>
static inline void addPreset(const std::string& name, PresetType&& preset,
    std::array<PresetType, MAX_PRESETS>& store, FindMap<Tag>& findMap, preset_tag::StrongID<Tag>& nextID) {
    preset_tag::StrongID<Tag> id;
    if (findMap.contains(name)) id = findMap[name];
    else                        id = nextID++;
    findMap.insert_or_assign(name, id);
    store[id.asUint()] = std::move(preset);
}

template<class PresetType>
void Presets::loadPresets(const std::string& folder, const Atlas& atlas) {
    namespace fs = std::filesystem;
    for (const auto& entry : fs::recursive_directory_iterator(io::folders::CONTENT / folder)) {
        const tin::Data data = tin::read(entry.path(), io::Log::only_error);
        const std::string name = entry.path().stem().string();
        const PresetReader reader(data, name);
        try {
            if constexpr (std::is_same_v<PresetType, BlockPreset>)
                addPreset(name, createBlockPreset(reader, atlas, turretFindMap, itemFindMap, name), blockStore, blockFindMap, nextBlockID);
            if constexpr (std::is_same_v<PresetType, ItemPreset>)
                addPreset(name, createItemPreset(reader, atlas), itemStore, itemFindMap, nextItemID);
            if constexpr (std::is_same_v<PresetType, OrePreset>)
                addPreset(name, createOrePreset(reader, atlas, itemFindMap), oreStore, oreFindMap, nextOreID);
            if constexpr (std::is_same_v<PresetType, MobPreset>)
                addPreset(name, createMobPreset(reader, atlas, turretFindMap), mobStore, mobFindMap, nextMobID);
            if constexpr (std::is_same_v<PresetType, ShellPreset>)
                addPreset(name, createShellPreset(reader, atlas), shellStore, shellFindMap, nextShellID);
            if constexpr (std::is_same_v<PresetType, TurretPreset>)
                addPreset(name, createTurretPreset(reader, atlas, shellFindMap, itemFindMap), turretStore, turretFindMap, nextTurretID);
            logger.debug("Preset created: {}", name);
        }
        catch (const std::bad_optional_access&) {
            logger.error("Failed to create preset (missing data in file): {}", name);
        }
        catch (const std::out_of_range&) {
            logger.error("Failed to create preset (dependency ID not found): {}", name);
        }
    }
}

void Presets::load(const Atlas& atlas) {
    loadPresets<ItemPreset>("items", atlas);
    loadPresets<OrePreset>("ores", atlas);
    loadPresets<ShellPreset>("shells", atlas);
    loadPresets<TurretPreset>("turrets", atlas);
    loadPresets<MobPreset>("mobs", atlas);
    loadPresets<BlockPreset>("blocks", atlas);
}
