#include "l_saves.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "app.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/folders.hpp"
#include "game/game_session.hpp"
#include "game/frontend/frontend.hpp"
#include "game/world_saver/world_saver.hpp"

constexpr Point BTN_SIZE(120, 30);

void FrSaves::update() {
    clear();
    auto contents = io::folders::getContents(io::folders::SAVES, io::folders::ContentsType::folder);
    for (const auto& it : contents) {
        auto btn = addNode(new Button(BTN_SIZE, it));
        btn->addCallback([&, it] { targetFolder = it; });
    }
}

void FrSaves::deleteWorld() {
    if (targetFolder != "")
        io::folders::deleteFolder(io::folders::SAVES / targetFolder);
    update();
}

void FrSaves::saveWorld(App& app, const std::string& folder) {
    // folder parameter prev was used to create new folder
    if (!io::folders::isPathValid(targetFolder))
        return;
    serializer::saveWorld(app.getSession().getWorld(), targetFolder);
    update();
    const fs::path absolutePath = fs::absolute(io::folders::SAVES / targetFolder);
    const std::string message = tr("World saved to {}", absolutePath.string());
    app.getGUI().addToOverlay(frontend::initMessageBox(app, message));
}

void FrSaves::loadWorld(App& app) const {
    app.loadWorldInGame(targetFolder);
}
