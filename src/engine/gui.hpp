#pragma once
#include <memory>
#include <vector>
#include "engine/widgets/container.hpp"

class MainWindow;

class GUI {
protected:
    std::vector<std::unique_ptr<Container>> containers;
    bool showGUI = true, showAtlas = false;
public:
    GUI() = default;
    virtual ~GUI() = default;

    void draw(const MainWindow& mainWindow);
    void acceptHotkeys(MainWindow& mainWindow);
protected:
    void relocateContainers(const PixelCoord windowSize);
    bool isMouseFree();
};
