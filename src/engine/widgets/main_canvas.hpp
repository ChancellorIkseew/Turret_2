#pragma once
#include "container.hpp"
#include "engine/io/parser/tin_parser.hpp"

class MainCanvas {
    std::vector<std::unique_ptr<Container>> mainLayer;
    std::vector<std::unique_ptr<Container>> overlay;
    tin::Data translations;
    PixelCoord windowSize;
    bool allwaysWithOverlay = false;
public:
    MainCanvas(const PixelCoord windowSize, tin::Data&& translations) :
        windowSize(windowSize), translations(std::move(translations)) {
    }

    void addToMainLayer(std::unique_ptr<Container> container) {
        prepareContainer(container);
        mainLayer.push_back(std::move(container));
    }
    void addToOverlay(std::unique_ptr<Container> container) {
        prepareContainer(container);
        overlay.push_back(std::move(container));
    }

    void update(UIContext& context) {
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

    void draw(const Renderer& renderer) {
        for (const auto& it : mainLayer) {
            it->draw(renderer);
        }
        if (hasOverlay())
            overlay.back()->draw(renderer);
    }

    void resize(const PixelCoord windowSize) {
        this->windowSize = windowSize;
        relocateContainers(windowSize);
    }

    bool hasOverlay() const {
        return !overlay.empty();
    }

    bool ownsMouse(const PixelCoord mousePosition) const {
        if (hasOverlay())
            return true;
        for (const auto& it : mainLayer) {
            if (it->containsMouse(mousePosition))
                return true;
        }
        return false;
    }

    void translate(tin::Data&& translations) {
        this->translations = std::move(translations);
        for (auto& it : mainLayer) it->translate(this->translations);
        for (auto& it : overlay)   it->translate(this->translations);
        relocateContainers(windowSize);
    }

    void setAllwaysWithOverlay(const bool flag) {
        allwaysWithOverlay = flag;
    }

    ///@brief It is safe even there is no overlay.
    void closeLastOverlaped() {
        if (hasOverlay() && (!allwaysWithOverlay || overlay.size() > 1))
            overlay.back()->close();
    }
private:
    void prepareContainer(std::unique_ptr<Container>& container) const {
        container->arrange();
        container->translate(translations);
        container->aplyAlignment(windowSize);
    }
    void relocateContainers(const PixelCoord windowSize) {
        for (const auto& it : mainLayer) it->aplyAlignment(windowSize);
        for (const auto& it : overlay)  it->aplyAlignment(windowSize);
    }
};
