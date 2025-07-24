#include "gui.hpp"
//
#include "engine/render/atlas.hpp"
#include "engine/widgets/form_editor/form_editor.hpp"
#include "engine/window/input/input.hpp"
#include "engine/window/window.hpp"

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
    overlaped.push_back(std::move(container));
}

void GUI::acceptHotkeys() {
    if (Input::jactive(Hide_GUI))
        showGUI = !showGUI;
    if (Input::jactive(Show_atlas))
        showAtlas = !showAtlas;
    if (Input::jactive(Fullscreen))
        mainWindow.setFullscreen(!mainWindow.isFullscreen());
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
    for (const auto& it : containers) {
        if (it->containsMouse())
            return false;
    }
    return true;
}
