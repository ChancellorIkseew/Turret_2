#pragma once
#include "gui.hpp"
//
#include "engine/window/input/input.hpp"
#include "game/frontend/build_tools/editor_build_tools.hpp"
#include "game/frontend/frontend.hpp"

class EditorGUI : public GUI {
    std::shared_ptr<BuildTools> buildTools;
public:
    EditorGUI(App& app) : GUI(app) { init(app); }

    void init(App& app) {
        mainCanvas.closeAll();
        buildTools = std::make_unique<EditorBuildTools>();
        mainCanvas.addToMainLayer(frontend::initJEI(app, buildTools));
    }

    void callback() final {
        buildTools->update(app);
        if (input.jactive(Escape) && !mainCanvas.hasOverlay())
            return GUI::addToOverlay(frontend::initMenu(app));
        GUI::callback();
    }

    void drawDiegeticElements(Renderer& renderer) final {
        buildTools->drawDraft(app, renderer, app.getMainWindow().getTimeMs());
    }
};
