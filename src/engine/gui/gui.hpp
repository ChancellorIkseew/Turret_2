#pragma once
#include <memory>
#include <vector>
#include "engine/engine.hpp"
#include "engine/settings/settings.hpp"
#include "engine/widgets/container.hpp"
#include "engine/widgets/main_canvas.hpp"
#include "engine/widgets/ui_context.hpp"

class GUI {
protected:
    Engine& engine;
    MainWindow& mainWindow;
    MainCanvas mainCanvas;
    Input& input;
    bool showGUI = true, showFPS = false, showAtlas = false;
public:
    GUI(Engine& engine) : engine(engine),
        mainWindow(engine.getMainWindow()),
        mainCanvas(mainWindow.getSize(), Settings::gui.lang),
        input(mainWindow.getInput()) { }
    virtual ~GUI() = default;

    void draw(const Renderer& renderer, const Atlas& atlas);
    void translate(const std::string& lang);
    virtual void callback();
    void addOverlaped(std::unique_ptr<Container> container);
    bool ownsMouse() const;
protected:
    void acceptHotkeys();
};
