#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/io/folders.hpp"
#include "engine/widgets/button.hpp"
#include "game/script_libs/lib_world_init.hpp"
#include "layouts/l_saves.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);
static std::string folder;

static void deleteFolder(Layout* saves, Container* loading) {
    io::folders::deleteFolder(io::folders::SAVES / folder);
    frontend::update(saves, folder);
    loading->arrange();
}

static void loadWorld(Engine& engine) {
    engine.loadWorldInGame(folder);
}

std::unique_ptr<Container> frontend::initWorldLoading(Engine& engine) {
    folder = ""; // Reset folder name to avoid erors.
    auto loading = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto saves = initSaves(folder);
    auto lower = std::make_unique<Layout>(Orientation::horizontal);

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto load = std::make_unique<Button>(BTN_SIZE, U"Load");
    auto deleteWorld = std::make_unique<Button>(BTN_SIZE, U"Delete");

    back->addCallback([container = loading.get()] { container->close(); });
    load->addCallback([&] { loadWorld(engine); });
    deleteWorld->addCallback(std::bind_front(deleteFolder, saves.get(), loading.get()));

    lower->addNode(back.release());
    lower->addNode(load.release());
    lower->addNode(deleteWorld.release());
    
    loading->addNode(saves.release());
    loading->addNode(lower.release());
    loading->arrange();
    return loading;
}
