#pragma once
#include <SDL3/SDL.h>
#include <atomic>
#include <filesystem>
#include <string>
#include "cursor.hpp"
#include "engine/coords/pixel_coord.hpp"

class MainWindow {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event = SDL_Event(0);
    Cursor cursor;
    Uint32 FPS = 60, requiredDelay = 16, realDelay = 0, frameStart = 0;
    bool resized = false, fullscreen = false;
    std::atomic_bool open = true;
public:
    MainWindow(const std::string& title);
    ~MainWindow();
    //
    void close() { open.store(false, std::memory_order_seq_cst); }
    void setFullscreen(const bool flag);
    void setFPS(const Uint32 FPS);
    bool isOpen() const { return open.load(std::memory_order_relaxed); }
    bool isFullscreen() const { return fullscreen; }
    bool justResized() const { return resized; }
    Uint32 getFPS() const { return FPS; }
    Uint32 getRealFrameDelay() const { return realDelay; }
    Uint64 getTime() const { return SDL_GetTicks(); }
    PixelCoord getSize() const {
        int x = 0, y = 0;
        SDL_GetWindowSize(window, &x, &y);
        return PixelCoord(x, y);
    }
    //
    void setCursor(const CursorType type) {
        if (getCursor() != type)
            cursor = Cursor(type);
    }
    CursorType getCursor() const { return cursor.getType(); }
    //
    void takeScreenshot(const std::filesystem::path& path) const;
    void pollEvents();
    void setRenderTranslation(const PixelCoord translation);
    void setRenderScale(const float scale) {
        SDL_SetRenderScale(renderer, scale, scale);
    }
    void clear() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
    void render() {
        SDL_RenderPresent(renderer);
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
