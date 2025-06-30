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
    virtual void acceptHotkeys(MainWindow& mainWindow);
    bool isMouseFree() const;
    virtual void callback() { }
protected:
    void relocateContainers(const PixelCoord windowSize);
};
