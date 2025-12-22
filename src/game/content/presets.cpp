#include "presets.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/debug/logger.hpp"
#include "game/physics/mob_manager.hpp"

static debug::Logger logger("presets");
std::unordered_map<std::string, csp::centralized_ptr<MobPreset>> content::Presets::mobPresets;

static csp::centralized_ptr<MobPreset> createPreset(const tin::Data& data) {
    MobVisualPreset visual(
        csp::make_centralized<Texture>(data.getString("texture").value()),
        data.getPixelCoord("origin").value(),
        data.getPixelCoord("size").value()
    );
    return csp::make_centralized<MobPreset>(
        data.getFloat("max_speed").value(),
        data.getFloat("hitbox_radius").value(),
        data.getInt64("max_health").value(),
        mob_ai::getMovingAI(data.getString("moving_ai").value()),
        mob_ai::getShootingAI(data.getString("shooting_ai").value()),
        visual
    );
}

void content::Presets::load() {
    const auto path = io::folders::CONTENT / "mobs";
    const auto contents = io::folders::getContents(path, io::folders::ContentsType::file);
    for (const auto& file : contents) {
        const auto data = tin::read(path / file, tin::Log::only_error);
        try {
            mobPresets.emplace(file.substr(0, file.length() - 4), createPreset(data));
            logger.debug() << "Mob preset created: " << file;
        }
        catch (const std::bad_optional_access&) {
            logger.error() << "Failed to create mob preset: " << file;
        }
    }
}
