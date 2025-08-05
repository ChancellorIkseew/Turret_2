#include "window.hpp"
//
#include <stdexcept>
#include "engine/render/atlas.hpp"
#include "engine/render/sprite.hpp"
#include "engine/render/text.hpp"
#include "input/input.hpp"

MainWindow::MainWindow(const std::string& title) {
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error("Could not init SDL");

    window = SDL_CreateWindow(title.c_str(), 720, 480, SDL_WINDOW_RESIZABLE);
    if (!window) {
        const std::string error = SDL_GetError();
        SDL_Quit();
        throw std::runtime_error("SDL_CreateWindow Error: " + error);
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        const std::string error = SDL_GetError();
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("SDL_CreateRenderer Error: " + error);
    }

    Input::init(window);
    Sprite::setRenderer(renderer);
    Atlas::setRenderer(renderer);
    text::setRenderer(renderer);
}

MainWindow::~MainWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MainWindow::setFPS(const Uint32 FPS) {
    this->FPS = FPS;
    requiredDelay = 1000U / FPS;
}

void MainWindow::setFullscreen(const bool flag) {
    fullscreen = flag;
    SDL_SetWindowFullscreen(window, flag);
}

void MainWindow::setRenderTranslation(const PixelCoord translation) {
    Sprite::setTranslation(translation);
}

void MainWindow::pollEvents() {
    Input::reset();
    resized = false;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            close();
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            resized = true;
            break;
        default:
            Input::update(event);
            break;
        }
    }
}
