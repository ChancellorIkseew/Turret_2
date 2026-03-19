#pragma once
#include "engine/io/parser/tin_parser.hpp"
#include "MINGUI/render/render_queue.hpp"
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
    template<IsPoint T>
    MainCanvas(const T windowSize, tin::Data&& translations) :
        MainCanvas(Point(windowSize), std::move(translations)) {}
    MainCanvas(const Point windowSize, tin::Data&& translations);
    //
    ///@brief It is safe even there is no overlay.
    void closeLastOverlaped();
    void addToMainLayer(std::unique_ptr<Container> container);
    void addToOverlay(std::unique_ptr<Container> container);
    //
    void update(UIContextBridge& contextBridge);
    ///@brief See RenderBridge.
    void drawBatched(RenderBridge& renderBridge);
    ///@brief See RenderBridge.
    void draw(RenderBridge& renderBridge);
    void translate(tin::Data&& translations);
    //
    template<IsPoint T>
    void resize(const T windowSize) { resize(Point(windowSize)); }
    void resize(const Point windowSize);
    //
    template<IsPoint T>
    bool ownsMouse(const T mousePosition) const { return ownsMouse(Point(mousePosition)); }
    bool ownsMouse(const Point mousePosition) const;
    //
    void setAllwaysWithOverlay(const bool flag) { allwaysWithOverlay = flag; }
    bool hasOverlay() const { return !overlay.empty(); }
private:
    void prepareContainer(std::unique_ptr<Container>& container) const;
    void relocateContainers(const Point windowSize);
};

END_NAMESPACE_MINGUI
