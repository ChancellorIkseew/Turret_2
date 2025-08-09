#include "frontend.hpp"
//
#include "engine/engine_state.hpp"
#include "engine/io/folders.hpp"
#include "engine/widgets/button.hpp"
#include "engine/window/input/utf8/utf8.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);
constexpr PixelCoord SMALL_BTN_SIZE(190.0f, 50.0f);
static std::string folder;

static void refreshFolders(Layout* folders, Container* saves) {
    folders->clear();
    auto contents = io::folders::getContents(io::folders::SAVES);
    for (const auto& it : contents) {
        auto btn = std::make_unique<Button>(SMALL_BTN_SIZE, utf8::fromConstCharToU32String(it.c_str()));
        btn->addCallback([&, it] { folder = it; });
        folders->addNode(btn.release());
    }
    folders->arrange();
    saves->arrange();
}

static void deleteFolder(Layout* folders, Container* saves) {
    io::folders::deleteFolder(io::folders::SAVES / folder);
    refreshFolders(folders, saves);
}

std::unique_ptr<Container> frontend::initSaves(EngineState& state) {
    auto saves = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto load = std::make_unique<Button>(BTN_SIZE, U"Load");
    auto deleteF = std::make_unique<Button>(BTN_SIZE, U"Delete");
    auto folders = std::make_unique<Layout>(Orientation::vertical);

    refreshFolders(folders.get(), saves.get());

    back->addCallback([container = saves.get()] { container->close(); });
    load->addCallback([&] { state = EngineState::gameplay; });
    deleteF->addCallback(std::bind_front(deleteFolder, folders.get(), saves.get()));

    saves->addNode(back.release());
    saves->addNode(folders.release());
    saves->addNode(load.release());
    saves->addNode(deleteF.release());
    saves->arrange();
    return saves;
}
