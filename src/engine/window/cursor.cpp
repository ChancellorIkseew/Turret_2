#include "cursor.hpp"
//
#include <SDL3/SDL_mouse.h>
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"

static debug::Logger logger("cursor");

Cursor::Cursor(const CursorType type) : type(type) {
    std::filesystem::path path;
    switch (type) {
    case CursorType::arrow:
        path = io::folders::CURSORS / "arrow.png";
        break;
    case CursorType::finger:
        path = io::folders::CURSORS / "finger.png";
        break;

    case CursorType::OS_default:
        SDL_SetCursor(SDL_GetDefaultCursor());
        return;
    // TODO: other types.
    }
    SDL_Surface* cursorSurface = SDL_LoadPNG(path.string().c_str());
    if (!cursorSurface)
        logger.error() << "Cold not Load image from file " << path;
    SDL_Cursor* cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
    SDL_DestroySurface(cursorSurface);
    SDL_SetCursor(cursor);
}

Cursor::~Cursor() {
    SDL_DestroyCursor(cursor);
}
