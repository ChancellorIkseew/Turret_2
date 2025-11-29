#pragma once
#include "cstdint"
#include "string"

class Engine;

struct _Display {
        _Display() = default;
        uint32_t FPS = 60U;
        bool fullscreen = false;
    };
    struct _Gameplay {
        _Gameplay() = default;
        bool cameraInertia = true;
        bool pauseOnWorldOpen = false;
        bool showHitboxes = false;
    };
    struct _GUI {
        _GUI() = default;
        std::string lang = "en_US";
        bool customCursor = true;
        bool showConsole = false;
    };
    
class Settings {
public:
    static inline _Display display;
    static inline _Gameplay gameplay;
    static inline _GUI gui;
    static void writeSettings();
    static void readSettings();
    static void aplySettings(Engine& engine);
};
