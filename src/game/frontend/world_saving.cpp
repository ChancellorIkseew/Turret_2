#include "frontend.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "game/world_saver/world_saver.hpp"
#include "game/script_libs/lib_world_init.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

static std::string fromU32StringToSTDString(std::u32string& u32str) {
    std::string str;
    str.reserve(u32str.size());
    for (const auto c : u32str) {
        str.append(1, static_cast<const char>(c));
    }
    return str;
}

static void saveWorld(Form* form) {
    std::string folder = fromU32StringToSTDString(form->getText());
    if (io::folders::isPathValid(folder))
        WorldSaver::save(*lib_world::world, folder);
}

std::unique_ptr<Container> frontend::initWorldSaving() {
    auto worldSaving = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto save = std::make_unique<Button>(BTN_SIZE, U"Save");
    auto worldName = std::make_unique<Form>();
    
    back->addCallback(std::bind(&Container::close, worldSaving.get()));
    save->addCallback(std::bind(saveWorld, worldName.get()));

    worldSaving->addNode(back.release());
    worldSaving->addNode(worldName.release());
    worldSaving->addNode(save.release());
    worldSaving->arrange();
    return worldSaving;
}
