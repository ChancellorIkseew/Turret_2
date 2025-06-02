#pragma once
#include <SDL3/SDL.h>
#include <stdexcept>

class MainWindow {
    SDL_Window* window;
    SDL_Renderer* renderer;
public:
    MainWindow() {
        if (!SDL_Init(SDL_INIT_VIDEO))
            throw std::runtime_error("Could not init SDL");

        window = SDL_CreateWindow("SDL3 Project", 720, 480, 0);
        if (!window) {
            const std::string eror = SDL_GetError();
            SDL_Quit();
            throw std::runtime_error("SDL_CreateWindow Error: " + eror);
        }

        renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer) {
            const std::string eror = SDL_GetError();
            SDL_DestroyWindow(window);
            SDL_Quit();
            throw std::runtime_error("SDL_CreateRenderer Error: " + eror);
        }
    }

    ~MainWindow() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void draw();
    void render();



    SDL_Renderer* getRenderer() { return renderer; };
};