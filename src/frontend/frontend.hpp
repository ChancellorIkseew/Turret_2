#pragma once
#include <memory>
#include "widgets/container.hpp"

class MainWindow;
class World;

namespace frontend {
    std::unique_ptr<Container> initMenu(MainWindow& mainWindow);
    std::unique_ptr<Container> initSettings();
}
