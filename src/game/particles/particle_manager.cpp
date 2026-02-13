#include "particle_manager.hpp"
//
#include <cmath>

void ParticleManager::reserve(const size_t capacity) {
    soa.position.reserve(capacity);
    soa.facing.reserve(capacity);
    soa.speed.reserve(capacity);
    soa.color.reserve(capacity);
    soa.restLifeTime.reserve(capacity);
}

void ParticleManager::addParticle(
    const PixelCoord position,
    const PixelCoord facing,
    const float speed,
    const uint32_t color,
    const TickCount restLifeTime) {

    soa.position.push_back(position);
    soa.facing.push_back(facing);
    soa.speed.push_back(speed);
    soa.color.push_back(color);
    soa.restLifeTime.push_back(restLifeTime);
    ++soa.particleCount;
}

void ParticleManager::addParticle(
    const PixelCoord position,
    const float angle,
    const float speed,
    const uint32_t color,
    const TickCount restLifeTime) {
    const PixelCoord facing(sinf(angle), cosf(angle));

    addParticle(position, facing, speed, color, restLifeTime);
}

void ParticleManager::removeParticle(const size_t index) {
    const size_t last = --soa.particleCount;

    if (index != last) {
        soa.position[index] = std::move(soa.position[last]);
        soa.facing[index] = std::move(soa.facing[last]);
        soa.speed[index] = std::move(soa.speed[last]);
        soa.color[index] = std::move(soa.color[last]);
        soa.restLifeTime[index] = std::move(soa.restLifeTime[last]);
    }

    soa.position.pop_back();
    soa.facing.pop_back();
    soa.speed.pop_back();
    soa.color.pop_back();
    soa.restLifeTime.pop_back();
}
