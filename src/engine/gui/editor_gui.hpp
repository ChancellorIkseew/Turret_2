#pragma once
#include "gui.hpp"
//
#include "engine/window/input/input.hpp"
#include "game/frontend/frontend.hpp"
#include "game/frontend/build_tools/editor_build_tools.hpp"

class EditorGUI : public GUI {
    std::shared_ptr<BuildTools> buildTools;
public:
    EditorGUI(Engine& engine) : GUI(engine) { init(engine); }

    void init(Engine& engine) {
        mainCanvas.closeAll();
        buildTools = std::make_unique<EditorBuildTools>();
        mainCanvas.addToMainLayer(frontend::initJEI(engine, buildTools));
    }

    void callback() final {
        buildTools->update(engine);
        if (input.jactive(Escape) && !mainCanvas.hasOverlay())
            return GUI::addToOverlay(frontend::initMenu(engine));
        GUI::callback();
    }

    void drawDiegeticElements(Renderer& renderer) final {
        buildTools->drawDraft(engine, renderer, engine.getMainWindow().getTimeMs());
    }
};
