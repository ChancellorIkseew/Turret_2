#pragma once
#include "MINGUI/widgets/container.hpp"
#include <memory>
#include "engine/io/parser/form_validator.hpp"

class BuildTools;
class Engine;

using namespace mingui;

namespace frontend {
    std::unique_ptr<Container> initMainMenu(Engine& engine);
    std::unique_ptr<Container> initMenu(Engine& engine);
    std::unique_ptr<Container> initControls(Engine& engine);
    std::unique_ptr<Container> initWorldLoading(Engine& engine);
    std::unique_ptr<Container> initWorldSaving(Engine& engine);
    std::unique_ptr<Container> initSettings(Engine& engine);
    std::unique_ptr<Container> initTimer(Engine& engine);
    std::unique_ptr<Container> initWorldProperties(Engine& engine);
    std::unique_ptr<Container> initJEI(Engine& engine, std::shared_ptr<BuildTools> buildTools);
    std::unique_ptr<Container> initHint(Engine& engine);
    std::unique_ptr<Container> initLanguages(Engine& engine);
    std::unique_ptr<Container> initGraphics(Engine& engine);
    std::unique_ptr<Container> initAudio(Engine& engine);
    std::unique_ptr<Container> initGUI(Engine& engine);
}
