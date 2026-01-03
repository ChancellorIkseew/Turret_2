#pragma once
#include <CSP/centralized_ptr.hpp>
#include <string>
#include <unordered_map>

struct MobPreset;
struct ShellPreset;
struct TurretPreset;

namespace content {
    class Presets {
        static std::unordered_map<std::string, csp::centralized_ptr<MobPreset>> mobPresets;
        static std::unordered_map<std::string, csp::centralized_ptr<ShellPreset>> shellPresets;
        static std::unordered_map<std::string, csp::centralized_ptr<TurretPreset>> turretPresets;
    public:
        static const auto& getMobs()    { return mobPresets; }
        static const auto& getShells()  { return shellPresets; }
        static const auto& getTurrets() { return turretPresets; }
        static void load();
    };
}
