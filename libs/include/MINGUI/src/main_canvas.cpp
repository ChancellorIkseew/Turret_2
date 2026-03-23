#include "MINGUI/core/main_canvas.hpp"

MINGUI

MainCanvas::MainCanvas(const Point windowSize, tin::Data&& translations) :
    windowSize(windowSize), translations(std::move(translations)) {
}

void MainCanvas::addToMainLayer(std::unique_ptr<Container> container) {
    prepareContainer(container);
    mainLayer.push_back(std::move(container));
}
void MainCanvas::addToOverlay(std::unique_ptr<Container> container) {
    prepareContainer(container);
    overlay.push_back(std::move(container));
}

void MainCanvas::update(UIContextBridge& contextBridge, const int frameDelay) {
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

void MainCanvas::translate(tin::Data&& translations) {
    this->translations = std::move(translations);
    for (auto& it : mainLayer) it->translate(this->translations);
    for (auto& it : overlay)   it->translate(this->translations);
    relocateContainers(windowSize);
}

void MainCanvas::closeLastOverlaped() noexcept {
    if (hasOverlay() && (!allwaysWithOverlay || overlay.size() > 1))
        overlay.back()->close();
}

void MainCanvas::prepareContainer(std::unique_ptr<Container>& container) const {
    container->arrange();
    container->translate(translations);
    container->aplyAlignment(windowSize);
}

void MainCanvas::relocateContainers(const Point windowSize) {
    for (const auto& it : mainLayer) it->aplyAlignment(windowSize);
    for (const auto& it : overlay)   it->aplyAlignment(windowSize);
}
