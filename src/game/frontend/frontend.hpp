#pragma once
#include <memory>
#include "engine/widgets/container.hpp"

enum class EngineState : uint8_t;
enum class TileType : uint8_t;
class MainWindow;
class World;

namespace frontend {
    std::unique_ptr<Container> initMainMenu(EngineState& state);
    std::unique_ptr<Container> initMenu(EngineState& state);
    std::unique_ptr<Container> initSettings();
    std::unique_ptr<Container> initTimer();
    std::unique_ptr<Container> initEditor();
    std::unique_ptr<Container> initJEI(TileType& tileType);
}
