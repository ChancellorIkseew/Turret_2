#pragma once
#include <filesystem>
#include <string>
#include "cursor.hpp"
#include "engine/render/renderer.hpp"
#include "engine/window/input/input.hpp"

struct SDL_Window;

struct SDLContext {
    SDL_Window* sdlWindow = nullptr;
    SDLContext(const std::string& title, const PixelCoord size);
    ~SDLContext(); // Not virtual because of no polymorphism.
};

class MainWindow : private SDLContext {
    PixelCoord size;
    Cursor cursor;
    Input input;
    Renderer renderer;
    uint32_t FPS = 60, requiredDelay = 16, realDelay = 0, frameStart = 0;
    bool open = true, resized = false, fullscreen = false;
public:
    MainWindow(const std::string& title, const PixelCoord size);
    //
    void close() { open = false; }
    void setFullscreen(const bool flag);
    void setFPS(const uint32_t FPS);
    bool isOpen() const { return open; }
    bool isFullscreen() const { return fullscreen; }
    bool justResized() const { return resized; }
    uint32_t getFPS() const { return FPS; }
    uint32_t getRealFrameDelay() const { return realDelay; }
    uint64_t getTime() const;
    PixelCoord getSize() const { return size; }
    //
    Cursor& getCursor() { return cursor; }
    Input& getInput() { return input; }
    Renderer& getRenderer() { return renderer; }
    //
    void takeScreenshot(const std::filesystem::path& path) const;
    void pollEvents();
    void render() {
        renderer.present(sdlWindow);
        makeDelay();
    }
private:
    void makeDelay();
    t1_disable_copy_and_move(MainWindow)
};
