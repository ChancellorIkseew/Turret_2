#include "l_saves.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/io/folders.hpp"
#include "game/world_saver/world_saver.hpp"

constexpr Point BTN_SIZE(120.0f, 30.0f);

void FrSaves::update() {
    clear();
    auto contents = io::folders::getContents(io::folders::SAVES, io::folders::ContentsType::folder);
    for (const auto& it : contents) {
        auto btn = addNode(new Button(BTN_SIZE, it, false));
        btn->addCallback([&, it] { targetFolder = it; });
    }
}

void FrSaves::deleteWorld() {
    io::folders::deleteFolder(io::folders::SAVES / targetFolder);
    update();
}

void FrSaves::saveWorld(Engine& engine, const std::string& folder) {
    if (!io::folders::isPathValid(folder))
        return;
    serializer::saveWorld(engine.getSession().getWorld(), folder);
    update();
}

void FrSaves::loadWorld(Engine& engine) const {
    engine.loadWorldInGame(targetFolder);
}
