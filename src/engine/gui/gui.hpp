#pragma once
#include <memory>
#include <stack>
#include <vector>
#include "engine/engine.hpp"
#include "engine/widgets/container.hpp"

class MainWindow;

class GUI {
protected:
    std::vector<std::unique_ptr<Container>> containers;
    std::deque<std::unique_ptr<Container>> overlaped;
    Engine& engine;
    MainWindow& mainWindow;
    bool showGUI = true, showAtlas = false;
public:
    GUI(Engine& engine) : engine(engine), mainWindow(engine.getMainWindow()) { }
    virtual ~GUI() = default;

    void draw();
    virtual void callback();
    void addOverlaped(std::unique_ptr<Container> container);
    bool isMouseFree() const;
protected:
    void relocateContainers();
    void acceptHotkeys();
};
