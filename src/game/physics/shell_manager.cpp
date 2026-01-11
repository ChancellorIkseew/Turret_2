#include "shell_manager.hpp"
//
#include <cmath>
#include "engine/assets/presets.hpp"

void ShellManager::reserve(const size_t capacity) {
    soa.position.reserve(capacity);
    soa.velocity.reserve(capacity);
    soa.angle.reserve(capacity);
    soa.restDamage.reserve(capacity);
    soa.restLifeTime.reserve(capacity);
    soa.teamID.reserve(capacity);
    soa.preset.reserve(capacity);
}

void ShellManager::addShell(
    const Presets& presets,
    const PresetID preset,
    const PixelCoord position,
    const AngleRad angle,
    const Health restDamage,
    const TickCount restLifeTime,
    const TeamID teamID) {
    const PixelCoord velocity = PixelCoord(sinf(angle), cosf(angle)) * presets.getShell(preset).speed;

    soa.position.push_back(position);
    soa.velocity.push_back(velocity);
    soa.angle.push_back(angle);
    soa.restDamage.push_back(restDamage);
    soa.restLifeTime.push_back(restLifeTime);
    soa.teamID.push_back(teamID);
    soa.preset.push_back(preset);
    ++soa.shellCount;
}

void ShellManager::removeShell(const size_t index) {
    const size_t last = soa.position.size() - 1;

    if (index != last) {
        soa.position[index] = std::move(soa.position[last]);
        soa.velocity[index] = std::move(soa.velocity[last]);
        soa.angle[index] = std::move(soa.angle[last]);
        soa.restDamage[index] = std::move(soa.restDamage[last]);
        soa.restLifeTime[index] = std::move(soa.restLifeTime[last]);
        soa.teamID[index] = std::move(soa.teamID[last]);
        soa.preset[index] = std::move(soa.preset[last]);
    }

    soa.position.pop_back();
    soa.velocity.pop_back();
    soa.angle.pop_back();
    soa.restDamage.pop_back();
    soa.restLifeTime.pop_back();
    soa.teamID.pop_back();
    soa.preset.pop_back();
    --soa.shellCount;
}
