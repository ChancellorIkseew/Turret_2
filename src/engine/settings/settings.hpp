#pragma once
#include <cstdint>
#include <string>

class Engine;

class Settings {
public:
    static struct _Audio {
        uint8_t master;
        uint8_t world;
        uint8_t ui;
        uint8_t music;
        bool toggleSound;
    } audio;
    static struct _Display {
        uint64_t FPS;
        bool ySinc;
        bool fullscreen;
    } display;
    static struct _Gameplay {
        bool cameraInertia;
        bool pauseOnWorldOpen;
        bool showHitboxes;
        bool showParticles;
        size_t maxParticles;
    } gameplay;
    static struct _GUI {
        std::string lang;
        uint8_t scale;
        bool customCursor;
        bool showConsole;
    } gui;
public:
    static void writeSettings();
    static void readSettings();
    static void applySettings(Engine& engine);
};
