#include "window.hpp"
//
#include <SDL3/SDL.h>
#include <stdexcept>
#include <cstring>
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "engine/render/atlas.hpp"

static std::filesystem::path ICON_PATH = io::folders::RES / "icon.png";
static debug::Logger logger("main_window");

static inline void loadIcon(SDL_Window* window) {
    Surface iconSurface(SDL_LoadPNG(ICON_PATH.string().c_str()));
    if (!iconSurface.raw())
        logger.error() << "Could not Load image from file " << ICON_PATH;
    SDL_SetWindowIcon(window, iconSurface.raw());
}

SDLContext::SDLContext(const std::string& title, const PixelCoord size) {
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error("Could not init SDL");
    sdlWindow = SDL_CreateWindow(title.c_str(), int(size.x), int(size.y), SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!sdlWindow) {
        const std::string error = SDL_GetError();
        SDL_Quit();
        throw std::runtime_error("SDL_CreateWindow Error: " + error);
    }
}

SDLContext::~SDLContext() {
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

MainWindow::MainWindow(const std::string& title, const PixelCoord size) :
    SDLContext(title, size), size(size), renderer(sdlWindow, size) {
    loadIcon(sdlWindow);
}

void MainWindow::setFPS(const uint64_t fps) {
    this->fps = fps;
    requiredDelayNs = NS_PER_SECOND / fps;
}

void MainWindow::setFullscreen(const bool flag) {
    fullscreen = flag;
    SDL_SetWindowFullscreen(sdlWindow, flag);
}

void MainWindow::pollEvents() {
    input.reset(sdlWindow);
    SDL_Event event;
    resized = lostFocus = gainedFocus = false;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            close();
            break;
        case SDL_EVENT_WINDOW_RESIZED:{
            resized = true;
            int x = 0, y = 0;
            SDL_GetWindowSize(sdlWindow, &x, &y);
            size = PixelCoord(x, y);
            renderer.resize(x, y);
            break;}
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            lostFocus = true;
            focused = false;
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            gainedFocus = true;
            focused = true;
            break;
        default:
            input.update(event);
            break;
        }
    }
    lostFocus = lostFocus && !focused;
    gainedFocus = gainedFocus && focused;
}

uint64_t MainWindow::getTimeMs() const {
    return SDL_GetTicks();
}

void MainWindow::makeDelay() {
    const uint64_t frameTimeNs = SDL_GetTicksNS() - frameStartNs;
    if (frameTimeNs < requiredDelayNs)
        SDL_DelayNS(requiredDelayNs - frameTimeNs);
    realDelayNs = std::max(frameTimeNs, requiredDelayNs);
    frameStartNs += realDelayNs;
}

void MainWindow::takeScreenshot(const std::filesystem::path& path) const {
    try {
        if (!io::folders::createOrCheckFolder(path.parent_path()))
            throw std::runtime_error("Failed to create or find directory.");

        // Получаем текущие размеры окна
        int width = 0;
        int height = 0;
        SDL_GetWindowSize(sdlWindow, &width, &height); // Ваше поле окна SDL

        // Получаем сырые данные из нового рендерера
        std::string rawPixels = renderer.takeScreenshot();

        // Создаем SDL_Surface поверх нашего буфера сырых пикселей.
        // SDL_PIXELFORMAT_RGBA32 идеально ложится на GL_RGBA + GL_UNSIGNED_BYTE
        Surface windowSurface(SDL_CreateSurfaceFrom(
            width, height,
            SDL_PIXELFORMAT_RGBA32,
            rawPixels.data(),
            width * 4 // шаг строки в байтах (pitch)
        ));

        if (!windowSurface.raw())
            throw std::runtime_error("SDL_CreateSurfaceFrom error: " + std::string(SDL_GetError()));

        // --- Переворачиваем картинку по вертикали (компенсация специфики OpenGL) ---
        // Так как данные лежат в rawPixels, переворачиваем строки прямо в Surface
        int pitch = windowSurface.raw()->pitch;
        std::vector<uint8_t> rowBuffer(pitch);
        uint8_t* pixelsPtr = static_cast<uint8_t*>(windowSurface.raw()->pixels);

        for (int i = 0; i < height / 2; ++i) {
            uint8_t* topRow = pixelsPtr + i * pitch;
            uint8_t* bottomRow = pixelsPtr + (height - 1 - i) * pitch;

            // Меняем строчки местами
            std::memcpy(rowBuffer.data(), topRow, pitch);
            std::memcpy(topRow, bottomRow, pitch);
            std::memcpy(bottomRow, rowBuffer.data(), pitch);
        }
        // ---------------------------------------------------------------------------

        // Сохраняем готовую правильную поверхность в файл
        if (!SDL_SavePNG(windowSurface.raw(), path.string().c_str()))
            throw std::runtime_error("SDL_SavePNG error: " + std::string(SDL_GetError()));
        logger.info() << "Screenshot saved. Path: " << path;
    }
    catch (const std::runtime_error& exception) {
        logger.error() << "Failed to take screenshot. " << exception.what();
    }
}
