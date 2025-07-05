#pragma once
#include <memory>
#include <vector>
#include "engine/widgets/container.hpp"

class MainWindow;

class GUI {
protected:
    std::vector<std::unique_ptr<Container>> containers;
    MainWindow& mainWindow;
    bool showGUI = true, showAtlas = false;
public:
    GUI(MainWindow& mainWindow) : mainWindow(mainWindow) { }
    virtual ~GUI() = default;

    void draw();
    bool isMouseFree() const;
    virtual void callback() = 0;
protected:
    void relocateContainers();
    void acceptHotkeys();
};
