#include "window.hpp"
//
#include <stdexcept>
#include "render/sprite.hpp"

MainWindow::MainWindow(const std::string& title) {
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error("Could not init SDL");

    window = SDL_CreateWindow(title.c_str(), 720, 480, SDL_WINDOW_RESIZABLE);
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

    Texture::setRenderer(renderer);
    Sprite::setRenderer(renderer);
}

MainWindow::~MainWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}