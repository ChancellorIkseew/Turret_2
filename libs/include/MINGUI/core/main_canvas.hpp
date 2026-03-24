#pragma once
#include "MINGUI/core/localization.hpp"
#include "MINGUI/core/text_edit.hpp"
#include "MINGUI/render/render_queue.hpp"
#include "MINGUI/widgets/container.hpp"

START_NAMESPACE_MINGUI

class MainCanvas {
    RenderQueue renderQueue;
    std::vector<std::unique_ptr<Container>> mainLayer;
    std::vector<std::unique_ptr<Container>> overlay;
    Localization localization;
    TextEdit textEdit;
    Palette palette;
    Point windowSize;
    bool allwaysWithOverlay = false;
public:
    template<IsPoint T>
    MainCanvas(const T windowSize, Localization&& localization, const Palette palette) :
        MainCanvas(Point(windowSize), std::move(localization), palette) {}
    MainCanvas(const Point windowSize, Localization&& localization, const Palette palette);
    //
    ///@brief It is safe even there is no overlay.
    void closeLastOverlaped() noexcept;
    void addToMainLayer(std::unique_ptr<Container> container);
    void addToOverlay(std::unique_ptr<Container> container);
    //
    void update(UIContextBridge& contextBridge, const int frameDelay);
    ///@brief See RenderBridge.
    void drawBatched(RenderBridge& renderBridge);
    ///@brief See RenderBridge.
    void draw(RenderBridge& renderBridge);
    void translate(Localization&& localization);
    //
    template<IsPoint T>
    void resize(const T windowSize) noexcept { resize(Point(windowSize)); }
    void resize(const Point windowSize) noexcept;
    //
    template<IsPoint T>
    bool ownsMouse(const T mousePosition) const noexcept { return ownsMouse(Point(mousePosition)); }
    bool ownsMouse(const Point mousePosition) const noexcept;
    //
    void setAllwaysWithOverlay(const bool flag) noexcept { allwaysWithOverlay = flag; }
    bool hasOverlay() const noexcept { return !overlay.empty(); }
    //
    bool isTextEditingActive() const noexcept { return textEdit.isEditingActive(); }
private:
    void refreshContainer(Container& container) const;
    void relocateContainers(const Point windowSize);
};

END_NAMESPACE_MINGUI
