#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/settings/settings.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/selector.hpp"
#include "engine/io/utf8/utf8.hpp"

constexpr PixelCoord LANG_BTN_SIZE(110.0f, 30.0f);
constexpr PixelCoord BACK_BTN_SIZE(120.0f, 30.0f);

static void changeLang(Engine& engine, const std::string& lang) {
    Settings::gui.lang = lang;
    Settings::writeSettings();
    GUI::loadLangTranslations(lang);
    engine.getGUI().translate();
}

std::unique_ptr<Container> frontend::initLanguages(Engine& engine) {
    auto languages = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto back = languages->addNode(new Button(BACK_BTN_SIZE, U"Back"));
    back->addCallback([container = languages.get()] { container->close(); });
    auto selector = languages->addNode(new Selector(Orientation::vertical));

    auto contents = io::folders::getContents(io::folders::LANG, io::folders::ContentsType::file);
    for (const auto& file : contents) {
        std::string lang = io::folders::trimExtensions(file);
        auto btn = selector->addNode(new Button(LANG_BTN_SIZE, utf8::to_u32string(lang), false));
        btn->addCallback([&, lang] { changeLang(engine, lang); });
        if (lang == Settings::gui.lang)
            selector->setTarget(btn);
    }

    languages->arrange();
    return languages;
}
