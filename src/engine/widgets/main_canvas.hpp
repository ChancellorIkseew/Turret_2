#pragma once
#include "container.hpp"
#include "engine/io/parser/tin_parser.hpp"

class MainCanvas {
    std::vector<std::unique_ptr<Container>> containers;
    std::vector<std::unique_ptr<Container>> overlaped;
    tin::Data translations;
    PixelCoord windowSize;
    bool allwaysWithOverlay = false;
public:
    MainCanvas(const PixelCoord windowSize, tin::Data&& translations) :
        windowSize(windowSize), translations(translations) { }

    void addContainer(std::unique_ptr<Container> container) {
        container->translate(translations);
        container->aplyAlignment(windowSize);
        containers.push_back(std::move(container));
    }
    void addOverlaped(std::unique_ptr<Container> container) {
        container->translate(translations);
        container->aplyAlignment(windowSize);
        overlaped.push_back(std::move(container));
    }

    void update(UIContext& context) {
        if (!overlaped.empty()) {
            overlaped.back()->callback(context);
            if (!overlaped.back()->isOpen())
                overlaped.pop_back();
            return; // Do not callback other containers.
        }
        for (const auto& it : containers) {
            it->callback(context);
        }
    }

    void draw(const Renderer& renderer) {
        for (const auto& it : containers) {
            it->draw(renderer);
        }
        if (hasOverlaped())
            overlaped.back()->draw(renderer);
    }

    void resize(const PixelCoord windowSize) {
        this->windowSize = windowSize;
        relocateContainers(windowSize);
    }

    bool hasOverlaped() const {
        return !overlaped.empty();
    }

    bool ownsMouse(const Input& input) const {
        if (hasOverlaped())
            return true;
        for (const auto& it : containers) {
            if (it->containsMouse(input))
                return true;
        }
        return false;
    }

    void translate(tin::Data&& translations) {
        this->translations = std::move(translations);
        for (auto& it : containers) it->translate(this->translations);
        for (auto& it : overlaped)  it->translate(this->translations);
        relocateContainers(windowSize);
    }

    void setAllwaysWithOverlay(const bool flag) {
        allwaysWithOverlay = flag;
    }

    ///@brief It is safe even there is no overlay.
    void closeLastOverlaped() {
        if (hasOverlaped() && (!allwaysWithOverlay || overlaped.size() > 1))
            overlaped.back()->close();
    }
private:
    void relocateContainers(const PixelCoord windowSize) {
        for (const auto& it : containers) it->aplyAlignment(windowSize);
        for (const auto& it : overlaped)  it->aplyAlignment(windowSize);
    }
};
