#pragma once
#include <CSP/centralized_ptr.hpp>
#include <string>
#include <unordered_map>

struct MobPreset;
struct ShellPreset;

namespace content {
    class Presets {
        static std::unordered_map<std::string, csp::centralized_ptr<MobPreset>> mobPresets;
        static std::unordered_map<std::string, csp::centralized_ptr<ShellPreset>> shellPresets;
    public:
        static const auto& getMobs() { return mobPresets; }
        static const auto& getShells() { return shellPresets; }
        static void load();
    };
}
