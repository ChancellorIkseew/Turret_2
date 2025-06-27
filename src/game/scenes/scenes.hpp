#pragma once
#include "game/frontend/frontend.hpp"

enum class SceneType {
    MAIN_MENU,
    GAMEPLAY
};


class ScenesM {



public:
    void update() {
        SceneType scene = SceneType::MAIN_MENU;

        switch (scene) {
        case SceneType::MAIN_MENU:

            break;

        case SceneType::GAMEPLAY:

            break;
        }
    }


    void openMenu() {



    }


};