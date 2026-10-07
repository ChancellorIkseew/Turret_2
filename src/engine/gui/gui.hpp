#pragma once
#include "MINGUI/core/main_canvas.hpp"

class Atlas;
class App;
class Input;
class MainWindow;
class Renderer;

class GUI {
protected:
    App& app;
    MainWindow& mainWindow;
    mingui::MainCanvas mainCanvas;
    Input& input;
    bool showGUI = true, showAtlas = false;
public:
    GUI(App& app);
    virtual ~GUI() = default;
    virtual void init(App& app) = 0;

    void draw(Renderer& renderer, const Atlas& atlas);
    virtual void drawDiegeticElements(Renderer& renderer) = 0;
    virtual void callback();
    void addToOverlay(std::unique_ptr<mingui::Container> container);
    bool overlapsWorld() const { return mainCanvas.hasOverlay(); }
    bool ownsMouse() const;
    void setScale(const uint8_t scale) { mainCanvas.setScale(scale); }
protected:
    void acceptHotkeys();
};
