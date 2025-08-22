#pragma once
#include "cstdint"
#include "string"

class Engine;

class Settings {
    struct _GUI {
        std::string lang = "en_US";
    };
    struct _Display {
        uint32_t FPS = 60U;
    };
public:
    static inline _GUI gui;
    static inline _Display display;
    static void writeSettings();
    static void readSettings();
    static void aplySettings(Engine& engine);
};
