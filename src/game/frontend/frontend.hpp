#pragma once
#include <memory>
#include "engine/widgets/container.hpp"

struct TileData;
struct ElementRegistry;
class Engine;

namespace frontend {
    std::unique_ptr<Container> initMainMenu(Engine& engine);
    std::unique_ptr<Container> initMenu(Engine& engine);
    std::unique_ptr<Container> initControls();
    std::unique_ptr<Container> initWorldLoading(Engine& engine);
    std::unique_ptr<Container> initWorldSaving();
    std::unique_ptr<Container> initSettings(Engine& engine);
    std::unique_ptr<Container> initTimer();
    std::unique_ptr<Container> initWorldProperties(Engine& engine);
    std::unique_ptr<Container> initJEI(TileData& tileData, const ElementRegistry& reg);
    std::unique_ptr<Container> initHint(TileData& tileData, const ElementRegistry& reg);
    std::unique_ptr<Container> initLanguages(Engine& engine);
    std::unique_ptr<Container> initGraphics(Engine& engine);
}
