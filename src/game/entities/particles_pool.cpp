#include "particles_pool.hpp"
//
#include <cmath>

void ParticlesPool::reserve(const size_t capacity) {
    soa.position.reserve(capacity);
    soa.facing.reserve(capacity);
    soa.size.reserve(capacity);
    soa.angle.reserve(capacity);
    soa.speed.reserve(capacity);
    soa.color.reserve(capacity);
    soa.colorFading.reserve(capacity);
    soa.restLifeTime.reserve(capacity);
    soa.type.reserve(capacity);
}

void ParticlesPool::addParticle(
    const PixelCoord position,
    const PixelCoord size,
    const float angle,
    const float speed,
    const uint32_t color,
    const uint32_t colorFading,
    const TickCount restLifeTime,
    const PType type) {

    soa.position.push_back(position);
    soa.facing.emplace_back(sinf(angle), cosf(angle));
    soa.size.push_back(size);
    soa.angle.push_back(angle);
    soa.speed.push_back(speed);
    soa.color.push_back(color);
    soa.colorFading.push_back(colorFading);
    soa.restLifeTime.push_back(restLifeTime);
    soa.type.push_back(type);
    ++soa.particleCount;
}

void ParticlesPool::removeParticle(const size_t index) {
    const size_t last = --soa.particleCount;

    if (index != last) {
        soa.position[index] = std::move(soa.position[last]);
        soa.facing[index] = std::move(soa.facing[last]);
        soa.size[index] = std::move(soa.size[last]);
        soa.angle[index] = std::move(soa.angle[last]);
        soa.speed[index] = std::move(soa.speed[last]);
        soa.color[index] = std::move(soa.color[last]);
        soa.colorFading[index] = std::move(soa.colorFading[last]);
        soa.restLifeTime[index] = std::move(soa.restLifeTime[last]);
        soa.type[index] = std::move(soa.type[last]);
    }

    soa.position.pop_back();
    soa.facing.pop_back();
    soa.size.pop_back();
    soa.angle.pop_back();
    soa.speed.pop_back();
    soa.color.pop_back();
    soa.colorFading.pop_back();
    soa.restLifeTime.pop_back();
    soa.type.pop_back();
}
