#pragma once
#include <SDL3/SDL.h>
#include <filesystem>
#include <string>
#include "cursor.hpp"
#include "engine/render/renderer.hpp"
#include "engine/window/input/input.hpp"

struct SDLContext {
    SDL_Window* sdlWindow = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;
    SDL_Event event = SDL_Event{ };
    SDLContext(const std::string& title);
    ~SDLContext(); // Not virtual because of no polymorphism.
};

class MainWindow : private SDLContext {
    Cursor cursor;
    Input input;
    Renderer renderer;
    Uint32 FPS = 60, requiredDelay = 16, realDelay = 0, frameStart = 0;
    bool open = true, resized = false, fullscreen = false;
public:
    MainWindow(const std::string& title);
    //
    void close() { open = false; }
    void setFullscreen(const bool flag);
    void setFPS(const Uint32 FPS);
    bool isOpen() const { return open; }
    bool isFullscreen() const { return fullscreen; }
    bool justResized() const { return resized; }
    Uint32 getFPS() const { return FPS; }
    Uint32 getRealFrameDelay() const { return realDelay; }
    Uint64 getTime() const { return SDL_GetTicks(); }
    PixelCoord getSize() const {
        int x = 0, y = 0;
        SDL_GetWindowSize(sdlWindow, &x, &y);
        return PixelCoord(x, y);
    }
    //
    Cursor& getCursor() { return cursor; }
    Input& getInput() { return input; }
    Renderer& getRenderer() { return renderer; }
    //
    void takeScreenshot(const std::filesystem::path& path) const;
    void pollEvents();
    void setRenderTranslation(const PixelCoord translation) {
        renderer.setTranslation(translation);
    }
    void setRenderScale(const float scale) {
        SDL_SetRenderScale(sdlRenderer, scale, scale);
    }
    void clear() {
        SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
        SDL_RenderClear(sdlRenderer);
    }
    void render() {
        SDL_RenderPresent(sdlRenderer);
        makeDelay();
    }
private:
    void makeDelay() {
        const Uint32 frameTime = Uint32(getTime()) - frameStart;
        if (frameTime < requiredDelay)
            SDL_Delay(requiredDelay - frameTime);
        realDelay = frameTime > requiredDelay ? frameTime : requiredDelay;
        frameStart += realDelay;
    }
    t1_disable_copy_and_move(MainWindow)
};
