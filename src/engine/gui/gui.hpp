#pragma once
#include "MINGUI/core/main_canvas.hpp"

class Atlas;
class Engine;
class Input;
class MainWindow;
class Renderer;

class GUI {
protected:
    Engine& engine;
    MainWindow& mainWindow;
    mingui::MainCanvas mainCanvas;
    Input& input;
    bool showGUI = true, showFPS = false, showAtlas = false;
public:
    GUI(Engine& engine);
    virtual ~GUI() = default;

    void draw(const Renderer& renderer, const Atlas& atlas);
    void translate(const std::string& lang);
    virtual void callback();
    void addToOverlay(std::unique_ptr<mingui::Container> container);
    bool ownsMouse() const;
protected:
    void acceptHotkeys();
};
