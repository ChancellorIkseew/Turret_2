#pragma once
#include "engine/io/parser/tin_parser.hpp"
#include "MINGUI/render/ui_render.hpp"
#include "MINGUI/widgets/container.hpp"

START_NAMESPACE_MINGUI

class MainCanvas {
    RenderQueue renderQueue;
    std::vector<std::unique_ptr<Container>> mainLayer;
    std::vector<std::unique_ptr<Container>> overlay;
    tin::Data translations;
    Point windowSize;
    bool allwaysWithOverlay = false;
public:
    MainCanvas(const Point windowSize, tin::Data&& translations);
    MainCanvas(const float windowSizeX, const float windowSizeY, tin::Data&& translations);
    //
    ///@brief It is safe even there is no overlay.
    void closeLastOverlaped();
    void addToMainLayer(std::unique_ptr<Container> container);
    void addToOverlay(std::unique_ptr<Container> container);
    //
    void update(UIContextBridge& contextBridge);
    void draw(const Renderer& renderer); //?
    void translate(tin::Data&& translations);
    //
    void resize(const float windowSizeX, const float windowSizeY);
    void resize(const Point windowSize);
    //
    bool ownsMouse(const float mousePositionX, const float mousePositionY) const;
    bool ownsMouse(const Point mousePosition) const;
    //
    void setAllwaysWithOverlay(const bool flag) { allwaysWithOverlay = flag; }
    bool hasOverlay() const { return !overlay.empty(); }
private:
    void prepareContainer(std::unique_ptr<Container>& container) const;
    void relocateContainers(const Point windowSize);
};

END_NAMESPACE_MINGUI
