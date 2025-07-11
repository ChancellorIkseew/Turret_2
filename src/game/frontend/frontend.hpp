#pragma once
#include <memory>
#include "engine/widgets/container.hpp"

enum class EngineState : uint8_t;
struct TileData;
struct ElementRegistry;

namespace frontend {
    std::unique_ptr<Container> initMainMenu(EngineState& state);
    std::unique_ptr<Container> initMenu(EngineState& state);
    std::unique_ptr<Container> initControls();
    std::unique_ptr<Container> initSettings();
    std::unique_ptr<Container> initTimer();
    std::unique_ptr<Container> initEditor();
    std::unique_ptr<Container> initJEI(TileData& tileData, const ElementRegistry& reg);
}
