#include "l_saves.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "engine/io/folders.hpp"

constexpr Point BTN_SIZE(120, 30);

void FrSaves::refresh() {
    clear();
    auto contents = io::folders::getContents(io::folders::SAVES, io::folders::ContentsType::folder);
    for (const auto& it : contents) {
        auto btn = addNode(new Button(BTN_SIZE, it));
        btn->addCallback([&, it] {
            targetFolder = it;
            if (onSelect)
                onSelect(it);
            });
    }
}
