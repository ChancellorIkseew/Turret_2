#include "frontend.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "engine/window/input/utf8/utf8.hpp"
#include "game/script_libs/lib_world_init.hpp"
#include "game/world_saver/world_saver.hpp"
#include "layouts/l_saves.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);
static std::string folder;

static void saveWorld(Form* form, Selector* saves, Container* saving) {
    std::string folder = validator::toStdString(form->getText());
    if (!io::folders::isPathValid(folder))
        return;
    WorldSaver::save(*lib_world::world, folder);
    frontend::update(saves, folder);
    saving->arrange();
}

std::unique_ptr<Container> frontend::initWorldSaving() {
    folder = ""; // Reset folder name to avoid erors.
    auto saving = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto saves = saving->addNode(initSaves(folder).release());
    auto lower = saving->addNode(new Layout(Orientation::horizontal));

    auto back = lower->addNode(new Button(BTN_SIZE, U"Back"));
    auto save = lower->addNode(new Button(BTN_SIZE, U"Save"));
    auto worldName = lower->addNode(new Form());

    back->addCallback([container = saving.get()] { container->close(); });
    save->addCallback([=, saving = saving.get()] { saveWorld(worldName, saves, saving); });

    saving->arrange();
    return saving;
}
