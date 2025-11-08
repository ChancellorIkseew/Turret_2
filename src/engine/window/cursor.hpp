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
public:
    Cursor(const CursorType type);
    Cursor() = default; // MainWindow must be initialized without custom coursor.
    ~Cursor();
    //
    CursorType getType() const { return type; }
};
