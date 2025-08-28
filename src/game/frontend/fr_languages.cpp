#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/settings/settings.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/selector.hpp"
#include "engine/window/input/utf8/utf8.hpp"

constexpr PixelCoord BTN_SIZE(110.0f, 30.0f);

static void changeLang(Engine& engine, const std::u32string langName) {
    Settings::gui.lang = validator::toStdString(langName);
    Settings::writeSettings();
    GUI::loadLangTranslations(Settings::gui.lang);
    engine.getGUI().translate();
}

std::unique_ptr<Container> frontend::initLanguages(Engine& engine) {
    auto languages = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto back = languages->addNode(new Button(PixelCoord(120, 30), U"Back"));
    back->addCallback([container = languages.get()] { container->close(); });
    auto selector = languages->addNode(new Selector(Orientation::vertical));

    auto contents = io::folders::getContents(io::folders::LANG, io::folders::ContentsType::file);
    for (const auto& file : contents) {
        const auto fileName = utf8::fromConstCharToU32String(file.c_str());
        auto langName = fileName.substr(0, fileName.length() - 4);
        auto btn = selector->addNode(new Button(BTN_SIZE, langName));
        btn->addCallback([&, langName] {changeLang(engine, langName); });
        if (file == Settings::gui.lang + ".tin")
            btn->setState(ButtonState::checked);
    }

    languages->arrange();
    return languages;
}