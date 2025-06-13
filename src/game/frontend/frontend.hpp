#pragma once
#include <memory>
#include "engine/widgets/container.hpp"

class MainWindow;
class World;

namespace frontend {
    std::unique_ptr<Container> initMenu(MainWindow& mainWindow);
    std::unique_ptr<Container> initSettings();
    std::unique_ptr<Container> initTimer();
    std::unique_ptr<Container> initEditor();
}
