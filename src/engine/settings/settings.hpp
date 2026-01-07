#pragma once
#include <cstdint>
#include <string>

class Engine;

namespace t1_sp { // sp - settings_private.
    // Private namespace instead of nested structures because
    // of GCC and Clang bug with nested structures/classes.
    struct _Audio {
        uint16_t master = 50U;
        uint16_t world  = 100U;
        uint16_t ui     = 100U;
        uint16_t music  = 100U;
        bool toggleSound = true;
    };
    struct _Display {
        uint32_t FPS = 60U;
        bool fullscreen = false;
    };
    struct _Gameplay {
        bool cameraInertia = true;
        bool pauseOnWorldOpen = false;
        bool showHitboxes = false;
    };
    struct _GUI {
        std::string lang = "en_US";
        bool customCursor = true;
        bool showConsole = false;
    };
}

class Settings {
public:
    static inline t1_sp::_Audio audio;
    static inline t1_sp::_Display display;
    static inline t1_sp::_Gameplay gameplay;
    static inline t1_sp::_GUI gui;
    static void writeSettings();
    static void readSettings();
    static void aplySettings(Engine& engine);
};
