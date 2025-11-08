#pragma once
#include "cstdint"
#include "string"

class Engine;

class Settings {
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
public:
    static inline _Display display;
    static inline _Gameplay gameplay;
    static inline _GUI gui;
    static void writeSettings();
    static void readSettings();
    static void aplySettings(Engine& engine);
};
