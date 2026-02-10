#pragma once
#include <array>
#include <cstdint>

enum class Event : uint8_t {
    map_changed,
    floor_changed,
    overlay_changed,
    block_changed,
    mob_spavned,
    shell_spawned,
    count
};

class Events {;
    static inline std::array<bool, static_cast<size_t>(Event::count)> states = {
        false, false, false, false, false, false
    };
public:
    static bool active(const Event eventType) {
        return states[size_t(eventType)];
    }
    static constexpr void setActive(const Event event) {
        states[size_t(event)] = true;
    }
    static void reset() {
        for (auto& state : states) {
            state = false;
        }
    }
};
