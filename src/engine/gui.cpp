#include "gui.hpp"
//
#include "frontend/frontend.hpp"
#include "window/input/input.hpp"
#include "window/window.hpp"

GUI::GUI(MainWindow& mainWindow) {
    containers.push_back(frontend::initMenu(mainWindow));
    relocateContainers(mainWindow.getSize());
}

void GUI::draw(const MainWindow& mainWindow) {
    if (Input::jactive(BindName::Hide_GUI))
        hideGUI();
    if (mainWindow.justResized())
        relocateContainers(mainWindow.getSize());
    for (const auto& it : containers) {
        it->draw();
        it->callback();
    }
}

void GUI::relocateContainers(const PixelCoord windowSize) {
    for (const auto& it : containers) {
        it->aplyAlignment(windowSize);
    }
}

void GUI::hideGUI() {
    for (const auto& it : containers) {
        it->setVisible(false);
    }
}
