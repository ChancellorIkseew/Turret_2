#pragma once
#include <cstdint>
#include <string>

class Engine;

class Settings {
public:
    static struct _Audio {
        uint16_t master = 50U;
        uint16_t world  = 100U;
        uint16_t ui     = 100U;
        uint16_t music  = 100U;
        bool toggleSound = true;
    } audio;
    static struct _Display {
        uint32_t FPS = 60U;
        bool fullscreen = false;
    } display;
    static struct _Gameplay {
        bool cameraInertia = true;
        bool pauseOnWorldOpen = false;
        bool showHitboxes = false;
    } gameplay;
    static struct _GUI {
        std::string lang = "en_US";
        bool customCursor = true;
        bool showConsole = false;
    } gui;
public:
    static void writeSettings();
    static void readSettings();
    static void applySettings(Engine& engine);
};
