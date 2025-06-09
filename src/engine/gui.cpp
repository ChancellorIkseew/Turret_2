#include "gui.hpp"
//
#include "frontend/frontend.hpp"

GUI::GUI() {
    containers.push_back(frontend::initMenu());
}

void GUI::draw() {
    for (const auto& it : containers) {
        it->draw();
    }
}
