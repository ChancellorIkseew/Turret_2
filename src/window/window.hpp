#pragma once
#include <SDL3/SDL.h>
#include <string>

class MainWindow {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint64 FPS = 60, requiredDelay = 16, frameStart = 0;
public:
    MainWindow(const std::string& title);
    ~MainWindow();
    //
    Uint64 getTime() const { return SDL_GetTicks(); }
    //
    void clear() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
    void render() {
        SDL_RenderPresent(renderer);
        makeDelay();
    }
    void setFPS(const Uint64 FPS) {
        this->FPS = FPS;
        requiredDelay = FPS / 1000;
    }
private:
    void makeDelay() {
        const Uint64 frameTime = getTime() - frameStart;
        if (frameTime < requiredDelay)
            SDL_Delay(requiredDelay - frameTime);
        //std::cout << requiredDelay - frameTime << '\n';
        frameStart = getTime();
    }
private:
    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;
};