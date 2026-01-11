#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "game/world_saver/world_saver.hpp"
#include "layouts/l_saves.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);
static std::string folder;

static void saveWorld(Form* form, Selector* saves, Container* saving, Engine& engine) {
    std::string folder = validator::trimToStdString(form->getText());
    if (!io::folders::isPathValid(folder))
        return;
    serializer::saveWorld(engine.getSession().getWorld(), folder);
    frontend::update(saves, folder);
    saving->arrange();
}

std::unique_ptr<Container> frontend::initWorldSaving(Engine& engine) {
    folder = ""; // Reset folder name to avoid erors.
    auto saving = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto saves = saving->addNode(initSaves(folder).release());
    auto lower = saving->addNode(new Layout(Orientation::horizontal));

    auto back = lower->addNode(new Button(BTN_SIZE, U"Back"));
    auto save = lower->addNode(new Button(BTN_SIZE, U"Save"));
    auto worldName = lower->addNode(new Form());

    back->addCallback([container = saving.get()] { container->close(); });
    save->addCallback([=, saving = saving.get(), &engine] { saveWorld(worldName, saves, saving, engine); });

    saving->arrange();
    return saving;
}
