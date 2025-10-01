#include "l_saves.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/utf8/utf8.hpp"
#include "engine/widgets/button.hpp"

constexpr PixelCoord BTN_SIZE(190.0f, 30.0f);

void frontend::update(Selector* saves, std::string& folder) {
    saves->clear();
    auto contents = io::folders::getContents(io::folders::SAVES, io::folders::ContentsType::folder);
    for (const auto& it : contents) {
        auto btn = saves->addNode(new Button(BTN_SIZE, utf8::to_u32string(it)));
        btn->addCallback([=, &folder] { folder = it; });
    }
    saves->arrange();
}

std::unique_ptr<Selector> frontend::initSaves(std::string& folder) {
    auto saves = std::make_unique<Selector>(Orientation::vertical);
    update(saves.get(), folder);
    return saves;
}
