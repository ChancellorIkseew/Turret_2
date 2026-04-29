#pragma once
#include "gui.hpp"
//
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/frontend/build_tools/editor_build_tools.hpp"

class EditorGUI : public GUI {
    std::shared_ptr<BuildTools> buildTools;
public:
    EditorGUI(Engine& engine) : GUI(engine) {
        buildTools = std::make_unique<EditorBuildTools>();
        mainCanvas.addToMainLayer(frontend::initJEI(engine, buildTools));
    }
    ~EditorGUI() final = default;

    void callback() final {
        if (input.jactive(Escape) && !mainCanvas.hasOverlay())
            GUI::addToOverlay(frontend::initMenu(engine));
        else
            GUI::callback();
    }

    void draw(Renderer& renderer, const Atlas& atlas) final {
        if (!ownsMouse())
            buildTools->drawBlueprint(engine, renderer);
        GUI::draw(renderer, atlas);
    }
};
