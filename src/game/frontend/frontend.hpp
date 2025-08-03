#pragma once
#include <memory>
#include "engine/widgets/container.hpp"

enum class EngineState : uint8_t;
struct TileData;
struct ElementRegistry;
class GUI;

namespace frontend {
    std::unique_ptr<Container> initMainMenu(EngineState& state, GUI& gui);
    std::unique_ptr<Container> initMenu(EngineState& state, GUI& gui);
    std::unique_ptr<Container> initControls();
    std::unique_ptr<Container> initSettings(GUI& gui);
    std::unique_ptr<Container> initTimer();
    std::unique_ptr<Container> initEditor();
    std::unique_ptr<Container> initJEI(TileData& tileData, const ElementRegistry& reg);
    std::unique_ptr<Container> initHint(TileData& tileData, const ElementRegistry& reg);
}
