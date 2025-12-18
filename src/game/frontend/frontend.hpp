#pragma once
#include <memory>
#include "engine/widgets/container.hpp"

struct TileData;
struct ContentIndexes;
class Engine;

namespace frontend {
    std::unique_ptr<Container> initMainMenu(Engine& engine);
    std::unique_ptr<Container> initMenu(Engine& engine);
    std::unique_ptr<Container> initControls(Engine& engine);
    std::unique_ptr<Container> initWorldLoading(Engine& engine);
    std::unique_ptr<Container> initWorldSaving(Engine& engine);
    std::unique_ptr<Container> initSettings(Engine& engine);
    std::unique_ptr<Container> initTimer(Engine& engine);
    std::unique_ptr<Container> initWorldProperties(Engine& engine);
    std::unique_ptr<Container> initJEI(TileData& tileData);
    std::unique_ptr<Container> initHint(TileData& tileData);
    std::unique_ptr<Container> initLanguages(Engine& engine);
    std::unique_ptr<Container> initGraphics(Engine& engine);
    std::unique_ptr<Container> initGUI(Engine& engine);
}
