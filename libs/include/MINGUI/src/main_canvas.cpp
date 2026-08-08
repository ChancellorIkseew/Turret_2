#include <MINGUI/core/main_canvas.hpp>
#include <MINGUI/render/render_bridge.hpp>

MINGUI

MainCanvas::MainCanvas(const Point windowSize, const float scale) :
    windowSize(windowSize), scale(scale), canvasSize(windowSize / scale) {}

void MainCanvas::addToMainLayer(std::unique_ptr<Container> container) {
    mainLayer.push_back(std::move(container));
}
void MainCanvas::addToOverlay(std::unique_ptr<Container> container) {
    overlay.push_back(std::move(container));
}

void MainCanvas::update(UIContextBridge& contextBridge, const int frameDelayMs) {
    textEdit.update(frameDelayMs);
    UIContext context(contextBridge, textEdit, scale);
    
    std::erase_if(mainLayer, [](auto& it) {return !it->isOpen();});
    std::erase_if(overlay,   [](auto& it) {return !it->isOpen();});

    if (!overlay.empty())
        overlay.back()->callback(context);
    else { // (overlay.empty)
        for (const auto& it : mainLayer) {
            it->callback(context);
        }
    }
    for (auto& it : mainLayer) refreshContainer(*it);
    for (auto& it : overlay)   refreshContainer(*it);
}

void MainCanvas::draw(RenderBridge& renderBridge) {
    for (const auto& it : mainLayer) {
        it->draw(renderQueue);
    }
    if (hasOverlay())
        overlay.back()->draw(renderQueue);
    renderBridge.setScale(scale);
    renderQueue.drawAndClear(renderBridge);
}

void MainCanvas::resize(const Point windowSize) noexcept {
    this->windowSize = windowSize;
    canvasSize = windowSize / scale;
    relocateContainers(canvasSize);
}

bool MainCanvas::ownsMouse(const Point mousePosition) const noexcept {
    if (hasOverlay())
        return true;
    for (const auto& it : mainLayer) {
        if (it->containsMouse(mousePosition / scale))
            return true;
    }
    return false;
}

void MainCanvas::setPaletteRecursive(const Palette& palette) {
    for (auto& it : mainLayer) it->setPaletteRecursive(palette);
    for (auto& it : overlay)   it->setPaletteRecursive(palette);
}

void MainCanvas::closeLastOverlaped() noexcept {
    if (hasOverlay() && (!allwaysWithOverlay || overlay.size() > 1))
        overlay.back()->close();
}
void MainCanvas::closeAll() noexcept {
    for (auto& it : mainLayer) it->close();
    for (auto& it : overlay)   it->close();
}

void MainCanvas::refreshContainer(Container& container) const {
    if (!container.isDirty())
        return;
    container.arrange();
    container.applyAlignment(canvasSize);
    container.applyAlignment(canvasSize); // temporary. needs bugfix
    container.markDirty(false);
}

void MainCanvas::relocateContainers(const Point canvasSize) {
    for (const auto& it : mainLayer) it->applyAlignment(canvasSize);
    for (const auto& it : overlay)   it->applyAlignment(canvasSize);
}
