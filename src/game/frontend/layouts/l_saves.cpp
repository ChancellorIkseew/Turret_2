#include "l_saves.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/widgets/button.hpp"
#include "engine/window/input/utf8/utf8.hpp"

constexpr PixelCoord BTN_SIZE(190.0f, 30.0f);

void frontend::update(Selector* saves, std::string& folder) {
    saves->clear();
    auto contents = io::folders::getContents(io::folders::SAVES, io::folders::ContentsType::folder);
    for (const auto& it : contents) {
        auto btn = saves->addNode(new Button(BTN_SIZE, utf8::fromConstCharToU32String(it.c_str())));
        btn->addCallback([=, &folder] { folder = it; });
    }
    saves->arrange();
}

std::unique_ptr<Selector> frontend::initSaves(std::string& folder) {
    auto saves = std::make_unique<Selector>(Orientation::vertical);
    update(saves.get(), folder);
    return saves;
}
