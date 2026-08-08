#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "MINGUI/widgets/selector.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/folders.hpp"
#include "engine/settings/settings.hpp"

constexpr Point LANG_BTN_SIZE(110.0f, 30.0f);
constexpr Point BACK_BTN_SIZE(116.0f, 30.0f);

static void changeLang(Engine& engine, const std::string& lang) {
    Settings::gui.lang = lang;
    Settings::applySettings(engine);
    Settings::writeSettings();
    engine.getGUI().init(engine);
}

std::unique_ptr<Container> frontend::initLanguages(Engine& engine) {
    auto languages = std::make_unique<Container>(Align::center, Orientation::vertical);
    auto back = languages->addNode(new Button(BACK_BTN_SIZE, tr("Back")));
    back->addCallback([container = languages.get()] { container->close(); });
    auto selector = languages->addNode(new Selector(Orientation::vertical));

    auto contents = io::folders::getContents(io::folders::LANG, io::folders::ContentsType::file);
    for (const auto& file : contents) {
        std::string lang = io::folders::trimExtensions(file);
        auto btn = selector->addNode(new Button(LANG_BTN_SIZE, lang));
        btn->addCallback([&, lang] { changeLang(engine, lang); });
        if (lang == Settings::gui.lang)
            selector->setTarget(btn);
    }

    return languages;
}
