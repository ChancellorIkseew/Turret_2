#pragma once
#include <typeinfo>

enum class EngineCommand : uint8_t {
    gameplay_load_world,
    gameplay_new_world,
    editor_load_world,
    editor_new_world,
    main_menu
};
