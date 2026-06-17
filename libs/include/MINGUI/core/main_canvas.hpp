#pragma once
#include <MINGUI/core/localization.hpp>
#include <MINGUI/core/text_edit.hpp>
#include <MINGUI/render/render_queue.hpp>
#include <MINGUI/widgets/container.hpp>

START_NAMESPACE_MINGUI

class MainCanvas {
    RenderQueue renderQueue;
    std::vector<std::unique_ptr<Container>> mainLayer;
    std::vector<std::unique_ptr<Container>> overlay;
    Localization localization;
    TextEdit textEdit;
    Palette palette;
    Point windowSize;
    Point canvasSize;
    bool allwaysWithOverlay = false;
    float scale;
public:
    template<IsPoint T>
    MainCanvas(const T windowSize, Localization&& localization, const Palette palette, const float scale) :
        MainCanvas(Point(windowSize), std::move(localization), palette, scale) {}
    MainCanvas(const Point windowSize, Localization&& localization, const Palette palette, const float scale);
    //
    ///@brief It is safe even there is no overlay.
    void closeLastOverlaped() noexcept;
    void addToMainLayer(std::unique_ptr<Container> container);
    void addToOverlay(std::unique_ptr<Container> container);
    //
    void update(UIContextBridge& contextBridge, const int frameDelayMs);
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
    void setScale(const float scale) noexcept { this->scale = scale; resize(windowSize); }
    float getScale() const noexcept { return scale; }
    //
    bool isTextEditingActive() const noexcept { return textEdit.isEditingActive(); }
private:
    void refreshContainer(Container& container) const;
    void relocateContainers(const Point canvasSize);
};

END_NAMESPACE_MINGUI
