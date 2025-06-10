#pragma once
#include <SDL3/SDL.h>
#include <string>
#include "coords/pixel_coord.hpp"

class MainWindow {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event = SDL_Event(0);
    Uint32 FPS = 60, requiredDelay = 16, frameStart = 0;
    bool open = true, resized = false;
public:
    MainWindow(const std::string& title);
    ~MainWindow();
    //
    void setFPS(const Uint32 FPS);
    bool isOpen() const { return open; }
    void close() { open = false; }
    bool justResized() const { return resized; }
    Uint64 getTime() const { return SDL_GetTicks(); }
    PixelCoord getSize() const {
        int x = 0, y = 0;
        SDL_GetWindowSize(window, &x, &y);
        return PixelCoord(x, y);
    }
    //
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
        frameStart = Uint32(getTime());
    }
private:
    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;
};
