#pragma once
#include <SDL3/SDL.h>
#include <string>

class MainWindow {
    SDL_Window* window;
    SDL_Renderer* renderer;
public:
    MainWindow(const std::string& title);
    ~MainWindow();
    //
    void clear() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
    void render() {
        SDL_RenderPresent(renderer);
    }
    //
    SDL_Renderer* getRenderer() { return renderer; };
};