#include "gui.hpp"
//
#include "game/frontend/frontend.hpp"
#include "engine/render/atlas.hpp"
#include "engine/widgets/form_editor/form_editor.hpp"
#include "engine/window/input/input.hpp"
#include "engine/window/window.hpp"

GUI::GUI(MainWindow& mainWindow) {
    containers.push_back(frontend::initMenu(mainWindow));
    containers.push_back(frontend::initSettings());
    containers.push_back(frontend::initTimer());
    containers.push_back(frontend::initEditor());
    relocateContainers(mainWindow.getSize());
}

void GUI::draw(const MainWindow& mainWindow) {
    if (Input::jactive(BindName::Hide_GUI))
        showGUI = !showGUI;
    if (Input::jactive(BindName::Show_atlas))
        showAtlas = !showAtlas;
    if (mainWindow.justResized())
        relocateContainers(mainWindow.getSize());
    //
    if (showGUI) {
        for (const auto& it : containers) {
            it->draw();
            it->callback();
        }
    }
    if (showAtlas)
        Atlas::testDraw();
    FormEditor::editForm();
}

void GUI::relocateContainers(const PixelCoord windowSize) {
    for (const auto& it : containers) {
        it->aplyAlignment(windowSize);
    }
}
