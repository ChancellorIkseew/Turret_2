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
            it->callback();
        }
    }
    if (showAtlas)
        Atlas::testDraw();
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
}

bool GUI::isMouseFree() const {
    for (const auto& it : containers) {
        if (it->containsMouse() && it->isVisible())
            return false;
    }
    return true;
}
