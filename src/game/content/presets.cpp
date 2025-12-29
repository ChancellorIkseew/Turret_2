#include "presets.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/debug/logger.hpp"
#include "game/physics/mob_manager.hpp"
#include "game/physics/shell_manager.hpp"

static debug::Logger logger("presets");
std::unordered_map<std::string, csp::centralized_ptr<MobPreset>>    content::Presets::mobPresets;
std::unordered_map<std::string, csp::centralized_ptr<ShellPreset>>  content::Presets::shellPresets;
std::unordered_map<std::string, csp::centralized_ptr<TurretPreset>> content::Presets::turretPresets;

static auto createMobPreset(const tin::Data& data) {
    MobVisualPreset visual(
        csp::make_centralized<Texture>(data.getString("texture").value()),
        data.getPixelCoord("origin").value(),
        data.getPixelCoord("size").value()
    );
    return csp::make_centralized<MobPreset>(
        data.getFloat("speed").value(),
        data.getFloat("hitbox_radius").value(),
        data.getInt64("health").value(),
        mob_ai::getMovingAI(data.getString("moving_ai").value()),
        content::Presets::getTurrets().at(data.getString("turret").value()),
        visual
    );
}

static auto createShellPreset(const tin::Data& data) {
    ShellVisualPreset visual(
        csp::make_centralized<Texture>(data.getString("texture").value()),
        data.getPixelCoord("origin").value(),
        data.getPixelCoord("size").value()
    );
    Explosion explosion(
        data.getInt64("explosion_damage").value(),
        data.getInt32("explosion_radius").value(),
        data.getFloat("explosion_fading").value()
    );
    return csp::make_centralized<ShellPreset>(
        data.getFloat("speed").value(),
        data.getInt64("damage").value(),
        data.getUint16("life_time").value(),
        explosion,
        visual
    );
}

static auto createTurretPreset(const tin::Data& data) {
    auto b = data.getList("barrels");
    const size_t barrelsCount = b.size();
    std::array<PixelCoord, 4> barrels;
    for (size_t i = 0; i < barrelsCount; ++i) {
        barrels[i] = validator::toPixelCoord(b[i]).value();
    }
    TurretVisualPreset visual(
        csp::make_centralized<Texture>(data.getString("texture").value()),
        data.getPixelCoord("origin").value(),
        data.getPixelCoord("size").value()
    );
    return csp::make_centralized<TurretPreset>(
        data.getUint16("reload").value(),
        barrelsCount,
        barrels,
        content::Presets::getShells().at(data.getString("shell").value()),
        visual
    );
}

template<typename Preset>
static void loadPresets(std::unordered_map<std::string, csp::centralized_ptr<Preset>>& presets, const std::string& folder) {
    const auto path = io::folders::CONTENT / folder;
    const auto contents = io::folders::getContents(path, io::folders::ContentsType::file);
    for (const auto& file : contents) {
        const auto data = tin::read(path / file, tin::Log::only_error);
        try {
            if constexpr (std::is_same_v<Preset, MobPreset>)
                presets.emplace(io::folders::trimExtensions(file), createMobPreset(data));
            if constexpr (std::is_same_v<Preset, ShellPreset>)
                presets.emplace(io::folders::trimExtensions(file), createShellPreset(data));
            if constexpr (std::is_same_v<Preset, TurretPreset>)
                presets.emplace(io::folders::trimExtensions(file), createTurretPreset(data));
            logger.debug() << "Preset created: " << file;
        }
        catch (const std::bad_optional_access&) {
            logger.error() << "Failed to create preset: " << file;
        }
    }
}

void content::Presets::load() {
    loadPresets(shellPresets, "shells");
    loadPresets(turretPresets, "turrets");
    loadPresets(mobPresets, "mobs");
}
