#pragma once
#include <memory>
#include <stack>
#include <vector>
#include "engine/engine.hpp"
#include "engine/widgets/container.hpp"
#include "engine/widgets/label.hpp"

class MainWindow;

class GUI {
protected:
    std::vector<std::unique_ptr<Container>> containers;
    std::deque<std::unique_ptr<Container>> overlaped;
    Label debugText;
    Engine& engine;
    MainWindow& mainWindow;
    bool showGUI = true, showFPS = false, showAtlas = false;
public:
    GUI(Engine& engine) : debugText(U""), engine(engine), mainWindow(engine.getMainWindow()) {}
    virtual ~GUI() = default;

    void draw();
    void translate();
    virtual void callback();
    void addOverlaped(std::unique_ptr<Container> container);
    bool hasOverlaped() const { return !overlaped.empty(); }
    bool isMouseFree() const;
    static void loadLangTranslations(const std::string& lang);
protected:
    void relocateContainers();
    void acceptHotkeys();
};
