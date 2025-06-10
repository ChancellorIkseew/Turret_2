#pragma once
#include <memory>
#include <vector>
#include "widgets/container.hpp"

class MainWindow;

class GUI {
    std::vector<std::unique_ptr<Container>> containers;
public:
    GUI(MainWindow& mainWindow);
    void draw(const MainWindow& mainWindow);
private:
    void relocateContainers(const PixelCoord windowSize);
    void hideGUI();
};
