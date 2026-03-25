#include <MINGUI/core/main_canvas.hpp>

MINGUI

MainCanvas::MainCanvas(const Point windowSize, Localization&& localization, const Palette palette) :
    windowSize(windowSize), localization(std::move(localization)), palette(palette) {
}

void MainCanvas::addToMainLayer(std::unique_ptr<Container> container) {
    mainLayer.push_back(std::move(container));
}
void MainCanvas::addToOverlay(std::unique_ptr<Container> container) {
    overlay.push_back(std::move(container));
}

void MainCanvas::update(UIContextBridge& contextBridge, const int frameDelay) {
    for (auto& it : mainLayer) refreshContainer(*it);
    for (auto& it : overlay)   refreshContainer(*it);

    textEdit.update(frameDelay);
    UIContext context(contextBridge, textEdit);
    if (!overlay.empty()) {
        overlay.back()->callback(context);
        if (!overlay.back()->isOpen())
            overlay.pop_back();
        return; // Do not callback other containers.
    }
    for (const auto& it : mainLayer) {
        it->callback(context);
    }
}

void MainCanvas::drawBatched(RenderBridge& renderBridge) {
    for (const auto& it : mainLayer) {
        it->draw(renderQueue);
    }
    if (hasOverlay())
        overlay.back()->draw(renderQueue);
    renderQueue.drawBatchedAndClear(renderBridge);
}

void MainCanvas::draw(RenderBridge& renderBridge) {
    for (const auto& it : mainLayer) {
        it->draw(renderQueue);
    }
    if (hasOverlay())
        overlay.back()->draw(renderQueue);
    renderQueue.drawAndClear(renderBridge);
}

void MainCanvas::resize(const Point windowSize) noexcept {
    this->windowSize = windowSize;
    relocateContainers(windowSize);
}

bool MainCanvas::ownsMouse(const Point mousePosition) const noexcept {
    if (hasOverlay())
        return true;
    for (const auto& it : mainLayer) {
        if (it->containsMouse(mousePosition))
            return true;
    }
    return false;
}

void MainCanvas::translate(Localization&& localization) {
    this->localization = std::move(localization);
    for (auto& it : mainLayer) it->translate(this->localization);
    for (auto& it : overlay)   it->translate(this->localization);
    relocateContainers(windowSize);
}

void MainCanvas::closeLastOverlaped() noexcept {
    if (hasOverlay() && (!allwaysWithOverlay || overlay.size() > 1))
        overlay.back()->close();
}

void MainCanvas::refreshContainer(Container& container) const {
    if (!container.isDirty())
        return;
    if (!container.getPalette().isValid())
        container.setPalette(palette);
    container.applyPalette();
    container.arrange();
    container.translate(localization);
    container.applyAlignment(windowSize);
    container.markDirty(false);
}

void MainCanvas::relocateContainers(const Point windowSize) {
    for (const auto& it : mainLayer) it->applyAlignment(windowSize);
    for (const auto& it : overlay)   it->applyAlignment(windowSize);
}
