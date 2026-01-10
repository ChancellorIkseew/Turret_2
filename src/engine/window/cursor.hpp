#pragma once
#include <cstdint>

enum class CursorType : uint8_t {
    OS_default,
    none,
    arrow,
    finger,
    target_unit,
    shooting_aim,
    mining_aim
    // TODO: update
};

struct SDL_Cursor;

class Cursor {
    SDL_Cursor* cursor = nullptr;
    CursorType type = CursorType::OS_default;
    CursorType defaultType = CursorType::OS_default;
public:
    Cursor() = default;
    ~Cursor();
    //
    void setType(const CursorType type);
    CursorType getType() const { return type; }
private:
    Cursor(const Cursor&) = delete;
    Cursor(Cursor&&) = delete;
    Cursor& operator=(const Cursor&) = delete;
    Cursor& operator=(Cursor&&) = delete;
};
