#pragma once
#include "cstdint"
#include "string"

class Engine;

class Settings {
    struct _GUI {
        std::string lang = "en_US";
        bool showConsole = false;
    };
    struct _Display {
        uint32_t FPS = 60U;
        bool fullscreen = false;
    };
    struct _Gameplay {
        bool showHitboxes = false; 
    };
public:
    static inline _GUI gui;
    static inline _Display display;
    static inline _Gameplay gameplay;
    static void writeSettings();
    static void readSettings();
    static void aplySettings(Engine& engine);
};
