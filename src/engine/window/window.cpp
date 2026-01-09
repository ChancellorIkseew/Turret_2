#include "window.hpp"
//
#include <stdexcept>
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"

static std::filesystem::path ICON_PATH = io::folders::RES / "icon.png";
static debug::Logger logger("main_window");

static inline void loadIcon(SDL_Window* window) {
    SDL_Surface* iconSurface = SDL_LoadPNG(ICON_PATH.string().c_str());
    if (!iconSurface)
        logger.error() << "Cold not Load image from file " << ICON_PATH;
    SDL_SetWindowIcon(window, iconSurface);
    SDL_DestroySurface(iconSurface);
}

SDLContext::SDLContext(const std::string& title) {
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
}

SDLContext::~SDLContext() {
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

MainWindow::MainWindow(const std::string& title) :
    SDLContext(title), input(sdlWindow), renderer(sdlRenderer) {
    loadIcon(sdlWindow);
}

void MainWindow::setFPS(const Uint32 FPS) {
    this->FPS = FPS;
    requiredDelay = 1000U / FPS;
}

void MainWindow::setFullscreen(const bool flag) {
    fullscreen = flag;
    SDL_SetWindowFullscreen(sdlWindow, flag);
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
        if (!SDL_SavePNG(windowSurface, path.string().c_str()))
            throw std::runtime_error("IMG_SavePNG error: " + std::string(SDL_GetError()));
        logger.info() << "Screenshot saved. Path: " << path;
    }
    catch (const std::runtime_error& exception) {
        logger.error() << "Failed to take screensot. " << exception.what();
    }
    SDL_DestroySurface(windowSurface);
}
