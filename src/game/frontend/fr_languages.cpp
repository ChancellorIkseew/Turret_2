#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/selector.hpp>
#include "app.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/folders.hpp"
#include "engine/settings/settings.hpp"

constexpr Point LANG_BTN_SIZE(110, 30);
constexpr Point BACK_BTN_SIZE(116, 30);

static void changeLang(App& app, const std::string& lang) {
    Settings::gui.lang = lang;
    Settings::applySettings(app);
    Settings::writeSettings();
    app.getGUI().init(app);
}

std::unique_ptr<Container> frontend::initLanguages(App& app) {
    auto languages = std::make_unique<Container>(Align::center, Orientation::vertical);
    auto back = languages->addNode(new Button(BACK_BTN_SIZE, tr("Back")));
    back->addCallback([container = languages.get()] { container->close(); });
    auto selector = languages->addNode(new Selector(Orientation::vertical));

    auto contents = io::folders::getContents(io::folders::LANG, io::folders::ContentsType::file);
    for (const auto& file : contents) {
        std::string lang = io::folders::trimExtensions(file);
        auto btn = selector->addNode(new Button(LANG_BTN_SIZE, lang));
        btn->addCallback([&, lang] { changeLang(app, lang); });
        if (lang == Settings::gui.lang)
            selector->setTarget(btn);
    }

    return languages;
}
