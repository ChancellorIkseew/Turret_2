#pragma once
#include <unordered_map>

enum class Event : uint8_t {
    terrain_changed
};

class Events {
    static inline std::unordered_map<Event, bool> events;
public:
    static void pushEvent(const Event event) {
        events[event] = true;
    }
    static void clear() {
        for (auto& [Event, state] : events) {
            state = false;
        }
    }
    static bool active(const Event event) {
        return events.contains(event) && events.at(event);
    }
};
