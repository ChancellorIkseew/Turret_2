#include "presets.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/debug/logger.hpp"
#include "game/physics/mob_manager.hpp"
#include "game/physics/shell_manager.hpp"

static debug::Logger logger("presets");
std::unordered_map<std::string, csp::centralized_ptr<MobPreset>> content::Presets::mobPresets;
std::unordered_map<std::string, csp::centralized_ptr<ShellPreset>> content::Presets::shellPresets;

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
        mob_ai::getShootingAI(data.getString("shooting_ai").value()),
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

void content::Presets::load() {
    const auto path = io::folders::CONTENT / "mobs";
    const auto contents = io::folders::getContents(path, io::folders::ContentsType::file);
    for (const auto& file : contents) {
        const auto data = tin::read(path / file, tin::Log::only_error);
        try {
            mobPresets.emplace(io::folders::trimExtensions(file), createMobPreset(data));
            logger.debug() << "Mob preset created: " << file;
        }
        catch (const std::bad_optional_access&) {
            logger.error() << "Failed to create mob preset: " << file;
        }
    }

    const auto path2 = io::folders::CONTENT / "shells";
    const auto contents2 = io::folders::getContents(path2, io::folders::ContentsType::file);
    for (const auto& file : contents2) {
        const auto data = tin::read(path2 / file, tin::Log::only_error);
        try {
            shellPresets.emplace(io::folders::trimExtensions(file), createShellPreset(data));
            logger.debug() << "Shell preset created: " << file;
        }
        catch (const std::bad_optional_access&) {
            logger.error() << "Failed to create shell preset: " << file;
        }
    }
}
