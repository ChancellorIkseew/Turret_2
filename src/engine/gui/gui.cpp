#include "gui.hpp"
//
#include "engine/debug/console.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/render/atlas.hpp"
#include "engine/settings/settings.hpp"
#include "engine/widgets/form_editor/form_editor.hpp"
#include "engine/window/input/input.hpp"
#include "engine/window/window.hpp"

static tin::Data langTranslations;

void GUI::draw() {
    if (mainWindow.justResized())
        relocateContainers();
    //
    if (showGUI) {
        for (const auto& it : containers) {
            it->draw();
        }
        if (!overlaped.empty())
            overlaped.back()->draw();
    }
    if (showAtlas)
        Atlas::testDraw();
}

void GUI::translate() {
    for (auto& it : containers) {
        it->translate(langTranslations);
    }
    for (auto& it : overlaped) {
        it->translate(langTranslations);
    }
}

void GUI::loadLangTranslations(const std::string& lang) {
    langTranslations = tin::read(io::folders::LANG / (lang + ".tin"));
}

void GUI::callback() {
    acceptHotkeys();
    if (!showGUI)
        return;
    if (!overlaped.empty()) {
        overlaped.back()->callback();
        if (!overlaped.back()->isOpen() || Input::jactive(Escape))
            overlaped.pop_back();
        return; // Do not callback other containers.
    }
    for (const auto& it : containers) {
        it->callback();
    }
}

void GUI::addOverlaped(std::unique_ptr<Container> container) {
    container->aplyAlignment(mainWindow.getSize());
    container->translate(langTranslations);
    overlaped.push_back(std::move(container));
}

void GUI::acceptHotkeys() {
    if (Input::jactive(Hide_GUI))
        showGUI = !showGUI;
    if (Input::jactive(Screenshot))
        mainWindow.takeScreenshot(io::folders::SCREENSHOTS / "img.png");
    if (Input::jactive(Show_atlas))
        showAtlas = !showAtlas;
    if (Input::jactive(Show_hitboxes)) {
        Settings::gameplay.showHitboxes = !Settings::gameplay.showHitboxes;
        Settings::writeSettings();
    }
    if (Input::jactive(Show_console)) {
        debug::Console::setVisible(!debug::Console::isVisible());
        Settings::gui.showConsole = debug::Console::isVisible();
        Settings::writeSettings();
    }
    if (Input::jactive(Fullscreen)) {
        mainWindow.setFullscreen(!mainWindow.isFullscreen());
        Settings::display.fullscreen = mainWindow.isFullscreen();
        Settings::writeSettings();
    }
    FormEditor::editForm();
}

void GUI::relocateContainers() {
    for (const auto& it : containers) {
        it->aplyAlignment(mainWindow.getSize());
    }
    for (const auto& it : overlaped) {
        it->aplyAlignment(mainWindow.getSize());
    }
}

bool GUI::isMouseFree() const {
    if (!showGUI)
        return true;
    if (!overlaped.empty() && overlaped.back()->containsMouse())
        return false;
    for (const auto& it : containers) {
        if (it->containsMouse())
            return false;
    }
    return true;
}
