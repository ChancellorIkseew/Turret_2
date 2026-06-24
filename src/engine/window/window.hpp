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
    static constexpr uint64_t NS_PER_SECOND = 1'000'000'000, NS_PER_MS = 1'000'000;
    PixelCoord size;
    Cursor cursor;
    Input input;
    Renderer renderer;
    uint64_t FPS = 60, requiredDelayNs = NS_PER_SECOND / 60, realDelayNs = 0, frameStartNs = 0;
    bool open = true, resized = false, fullscreen = false, lostFocus = false, gainedFocus = false;
public:
    MainWindow(const std::string& title, const PixelCoord size);
    //
    void close() { open = false; }
    void setFullscreen(const bool flag);
    void setFPS(const uint64_t FPS);
    bool isOpen() const { return open; }
    bool isFullscreen() const { return fullscreen; }
    bool justResized() const { return resized; }
    bool hasLostFocus() const { return lostFocus; }
    bool hasGainedFocus() const { return gainedFocus; }
    uint64_t getFPS() const { return FPS; }
    uint64_t getRealFrameDelayNs() const { return realDelayNs; }
    uint64_t getRealFrameDelayMs() const { return realDelayNs / NS_PER_MS; }
    uint64_t getTimeMs() const;
    PixelCoord getSize() const { return size; }
    //
    Cursor& getCursor() { return cursor; }
    Input& getInput() { return input; }
    Renderer& getRenderer() { return renderer; }
    //
    void takeScreenshot(const std::filesystem::path& path) const;
    void pollEvents();
    void clear() {
        renderer.clear();
    }
    void render() {
        renderer.present(sdlWindow);
        makeDelay();
    }
private:
    void makeDelay();
    t1_disable_copy_and_move(MainWindow)
};
