#pragma once
#include <cstdint>

enum class EngineState : uint8_t {
    main_menu,
    gameplay,
    map_editor,
    exit
};
