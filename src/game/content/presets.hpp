#pragma once
#include <CSP/centralized_ptr.hpp>
#include <string>
#include <unordered_map>

struct MobPreset;

namespace content {
    class Presets {
        static std::unordered_map<std::string, csp::centralized_ptr<MobPreset>> mobPresets;
    public:
        static const std::unordered_map<std::string, csp::centralized_ptr<MobPreset>>& getMobs() {
            return mobPresets;
        }
        static void load();
    };
}
