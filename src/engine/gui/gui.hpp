#pragma once
#include <memory>
#include <vector>
#include "engine/engine.hpp"
#include "engine/widgets/container.hpp"
#include "engine/widgets/ui_context.hpp"

class GUI {
protected:
    std::vector<std::unique_ptr<Container>> containers;
    std::vector<std::unique_ptr<Container>> overlaped;
    Engine& engine;
    MainWindow& mainWindow;
    Input& input;
    bool showGUI = true, showFPS = false, showAtlas = false;
public:
    GUI(Engine& engine) : engine(engine),
        mainWindow(engine.getMainWindow()), input(mainWindow.getInput()) {}
    virtual ~GUI() = default;

    void draw(const Renderer& renderer, const Atlas& atlas);
    void translate();
    virtual void callback();
    void addOverlaped(std::unique_ptr<Container> container);
    bool hasOverlaped() const { return !overlaped.empty(); }
    bool isMouseFree() const;
    bool ownsMouse() const;
    static void loadLangTranslations(const std::string& lang);
protected:
    void relocateContainers();
    void acceptHotkeys();
};
