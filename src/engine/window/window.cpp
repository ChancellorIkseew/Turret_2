#include "window.hpp"
//
#include <SDL3_image/SDL_image.h>
#include <stdexcept>
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/sprite.hpp"
#include "engine/render/text.hpp"

static std::filesystem::path ICON_PATH = io::folders::RES / "icon.bmp";
static debug::Logger logger("main_window");

static inline void loadIcon(SDL_Window* window) {
    SDL_Surface* iconSurface = IMG_Load(ICON_PATH.string().c_str());
    if (!iconSurface)
        logger.error() << "Cold not Load image from file " << ICON_PATH;
    SDL_SetWindowIcon(window, iconSurface);
    SDL_DestroySurface(iconSurface);
}

MainWindow::MainWindow(const std::string& title) {
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error("Could not init SDL");

    sdlWindow = SDL_CreateWindow(title.c_str(), 720, 480, SDL_WINDOW_RESIZABLE);
    if (!sdlWindow) {
        const std::string error = SDL_GetError();
        SDL_Quit();
        throw std::runtime_error("SDL_CreateWindow Error: " + error);
    }

    sdlRenderer = SDL_CreateRenderer(sdlWindow, nullptr);
    if (!sdlRenderer) {
        const std::string error = SDL_GetError();
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        throw std::runtime_error("SDL_CreateRenderer Error: " + error);
    }

    loadIcon(sdlWindow);

    input.setWindow(sdlWindow);
    Sprite::setRenderer(sdlRenderer);
    Atlas::setRenderer(sdlRenderer);
    text::setRenderer(sdlRenderer);
}

MainWindow::~MainWindow() {
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void MainWindow::setFPS(const Uint32 FPS) {
    this->FPS = FPS;
    requiredDelay = 1000U / FPS;
}

void MainWindow::setFullscreen(const bool flag) {
    fullscreen = flag;
    SDL_SetWindowFullscreen(sdlWindow, flag);
}

void MainWindow::setRenderTranslation(const PixelCoord translation) {
    Sprite::setTranslation(translation);
}

void MainWindow::pollEvents() {
    input.reset();
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
            input.update(event);
            break;
        }
    }
}

void MainWindow::takeScreenshot(const std::filesystem::path& path) const {
    SDL_Surface* windowSurface = SDL_RenderReadPixels(sdlRenderer, nullptr);
    try {
        if (!io::folders::createOrCheckFolder(path.parent_path()))
            throw std::runtime_error("Failed to create or find directory.");
        if (!windowSurface)
            throw std::runtime_error("SDL_RenderReadPixels error: " + std::string(SDL_GetError()));
        if (!IMG_SavePNG(windowSurface, path.string().c_str()))
            throw std::runtime_error("IMG_SavePNG error: " + std::string(SDL_GetError()));
        logger.info() << "Screenshot saved. Path: " << path;
    }
    catch (const std::runtime_error& exception) {
        logger.error() << "Failed to take screensot. " << exception.what();
    }
    SDL_DestroySurface(windowSurface);
}
