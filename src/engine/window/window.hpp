#pragma once
#include <SDL3/SDL.h>
#include <filesystem>
#include <string>
#include "cursor.hpp"
#include "engine/coords/pixel_coord.hpp"
#include "engine/window/input/input.hpp"

class MainWindow {
    Input input;
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    SDL_Event event = SDL_Event(0);
    Cursor cursor;
    Uint32 FPS = 60, requiredDelay = 16, realDelay = 0, frameStart = 0;
    bool resized = false, fullscreen = false;
    bool open = true;
public:
    MainWindow(const std::string& title);
    ~MainWindow();
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
    void setCursor(const CursorType type) {
        if (getCursor() != type)
            cursor = Cursor(type);
    }
    CursorType getCursor() const { return cursor.getType(); }
    Input& getInput() { return input; }
    //
    void takeScreenshot(const std::filesystem::path& path) const;
    void pollEvents();
    void setRenderTranslation(const PixelCoord translation);
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
private:
    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;
};
