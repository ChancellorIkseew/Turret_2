#pragma once
#include <memory>
#include <vector>
#include "engine/widgets/container.hpp"

class MainWindow;

class GUI {
    std::vector<std::unique_ptr<Container>> containers;
    bool showGUI = true, showAtlas = false;
public:
    GUI(MainWindow& mainWindow);
    void draw(const MainWindow& mainWindow);
    void acceptHotkeys(MainWindow& mainWindow);
private:
    void relocateContainers(const PixelCoord windowSize);
};
