#include "frontend.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/parser/validator.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "engine/window/input/utf8/utf8.hpp"
#include "game/script_libs/lib_world_init.hpp"
#include "game/world_saver/world_saver.hpp"
#include "layouts/l_saves.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);
static std::string folder;

static void saveWorld(Form* form, Layout* saves, Container* saving) {
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
    auto saves = initSaves(folder);
    auto lower = std::make_unique<Layout>(Orientation::horizontal);

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto save = std::make_unique<Button>(BTN_SIZE, U"Save");
    auto worldName = std::make_unique<Form>();

    back->addCallback([container = saving.get()] { container->close(); });
    save->addCallback(std::bind_front(saveWorld, worldName.get(), saves.get(), saving.get()));

    lower->addNode(back.release());
    lower->addNode(save.release());
    lower->addNode(worldName.release());

    saving->addNode(saves.release());
    saving->addNode(lower.release());
    saving->arrange();
    return saving;
}
