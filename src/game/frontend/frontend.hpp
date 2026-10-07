#pragma once
#include <MINGUI/widgets/container.hpp>
#include <memory>
#include "engine/io/parser/form_validator.hpp"
#include "engine/settings/localization.hpp"

class BuildTools;
class App;

using namespace mingui;

namespace frontend {
    std::unique_ptr<Container> initMainMenu(App& app);
    std::unique_ptr<Container> initMenu(App& app);
    std::unique_ptr<Container> initControls(App& app);
    std::unique_ptr<Container> initWorldLoading(App& app);
    std::unique_ptr<Container> initWorldSaving(App& app);
    std::unique_ptr<Container> initSettings(App& app);
    std::unique_ptr<Container> initTimer(App& app);
    std::unique_ptr<Container> initGameOver(App& app);
    std::unique_ptr<Container> initWorldProperties(App& app);
    std::unique_ptr<Container> initInventory(App& app);
    std::unique_ptr<Container> initJEI(App& app, std::shared_ptr<BuildTools> buildTools);
    std::unique_ptr<Container> initBlockInfo(App& app, const uint8_t blockPresetID);
    std::unique_ptr<Container> initHint(App& app);
    std::unique_ptr<Container> initLanguages(App& app);
    std::unique_ptr<Container> initGameplay(App& app);
    std::unique_ptr<Container> initGraphics(App& app);
    std::unique_ptr<Container> initAudio(App& app);
    std::unique_ptr<Container> initGUI(App& app);

    inline void useLabelsSpacing(Layout* layout) {
        layout->setMargin(9.f);
        layout->setPadding(5.f);
    }
}
