#pragma once
#include <CSP/centralized_ptr.hpp>
#include <vector>
#include "engine/coords/transforms.hpp"
#include "engine/render/sprite.hpp"
#include "game/player/camera.hpp"
#include "physics_base.hpp"

using MobID = uint16_t;
using Angle = float;

struct VisualPreset {
    csp::centralized_ptr<Texture> texture;
    const PixelCoord origin;
    const PixelCoord size;
};

struct Preset {
    const float maxSpeed;
    const float hitboxRadius;
    const VisualPreset visual;
};

struct MobSoA {
    std::vector<Hitbox> hitbox;
    std::vector<PixelCoord> position;
    std::vector<PixelCoord> velocity;
    std::vector<Angle> angle;
    std::vector<csp::centralized_ptr<Preset>> preset;
    std::vector<MobID> id;
    std::vector<TeamID> teamID;
};

inline void reserve(MobSoA& soa, const size_t capacity) {
    soa.id.reserve(capacity);
    soa.position.reserve(capacity);
    soa.velocity.reserve(capacity);
    soa.angle.reserve(capacity);
    soa.teamID.reserve(capacity);
    soa.preset.reserve(capacity);
    soa.hitbox.reserve(capacity);
}

inline size_t addMob(MobSoA& soa,
    const csp::centralized_ptr<Preset>& preset,
    const PixelCoord pos,
    const Angle angle,
    const TeamID teamID,
    const MobID mobID) {

    soa.id.push_back(mobID);
    soa.position.push_back(pos);
    soa.velocity.push_back(PixelCoord(0, 0));
    soa.angle.push_back(angle);
    soa.teamID.push_back(teamID);
    soa.preset.push_back(preset);
    soa.hitbox.push_back(Hitbox(pos, preset->hitboxRadius));

    return soa.id.size() - 1; // Возвращаем текущий индекс
}

inline void removeMob(MobSoA& soa, size_t index) {
    const size_t last = soa.id.size() - 1;

    if (index != last) {
        soa.id[index] = std::move(soa.id[last]);
        soa.position[index] = std::move(soa.position[last]);
        soa.velocity[index] = std::move(soa.velocity[last]);
        soa.angle[index] = std::move(soa.angle[last]);
        soa.teamID[index] = std::move(soa.teamID[last]);
        soa.preset[index] = std::move(soa.preset[last]);
        soa.hitbox[index] = std::move(soa.hitbox[last]);
    }

    soa.id.pop_back();
    soa.position.pop_back();
    soa.velocity.pop_back();
    soa.angle.pop_back();
    soa.teamID.pop_back();
    soa.preset.pop_back();
    soa.hitbox.pop_back();
}

static t1_finline void move(MobSoA& soa, const size_t index, const PixelCoord vector) {
    soa.position[index] = soa.position[index] + vector;
    soa.hitbox[index].move(vector);
}

static inline void resolveCollisions(MobSoA& soa, const size_t current, const size_t end) {
    for (size_t other = current + 1; other < end; ++other) {
        if (!soa.hitbox[current].intersects(soa.hitbox[other])) continue;

        const PixelCoord overlap = soa.hitbox[current].overlap(soa.hitbox[other]);

        const bool pushX = (overlap.x < overlap.y);
        const float distance = pushX ? overlap.x : overlap.y;

        const float pushDirection = pushX ? (soa.position[current].x > soa.position[other].x ? 1.0f : -1.0f)
            : (soa.position[current].y > soa.position[other].y ? 1.0f : -1.0f);

        const float totalRadius = soa.preset[current]->hitboxRadius + soa.preset[other]->hitboxRadius;
        const float invertedTotalRadius = 1.0f / totalRadius;
        const float mob1Weight = soa.preset[other]  ->hitboxRadius * invertedTotalRadius;
        const float mob2Weight = soa.preset[current]->hitboxRadius * invertedTotalRadius;

        PixelCoord pushVec;
        if (pushX) pushVec.x = distance * pushDirection;
        else /*Y*/ pushVec.y = distance * pushDirection;

        move(soa, current, pushVec * mob1Weight);
        move(soa, other,   pushVec * -mob2Weight);
    }
}

static inline void moveByAI(MobSoA& soa, const size_t index) {
    if (false)
        return;
    //updateAI
    PixelCoord AIVector; //getFromAI
    Angle AIAngle = 0; //getFromAI
    soa.velocity[index] = AIVector * soa.preset[index]->maxSpeed;
    soa.angle[index] = AIAngle;
    move(soa, index, soa.velocity[index]);
}

inline void processMobs(MobSoA& soa) {
    size_t end = soa.id.size();
    for (size_t i = 0; i < end; ++i) {
        resolveCollisions(soa, i, end);
    }
}

inline void drawMobs(const MobSoA& soa, const Camera& camera, const float tickOfset) {
    Sprite sprite;
    for (size_t i = 0; i < soa.id.size(); ++i) {
        if (!camera.contains(t1::tile(soa.position[i])))
            return;
        auto& visual = soa.preset[i]->visual;
        sprite.setTexture(*visual.texture);
        sprite.setOrigin(visual.origin);
        sprite.setSize(visual.size);
        sprite.setPosition(soa.position[i] + soa.velocity[i] * tickOfset);
        sprite.setRotationRad(soa.angle[i]);
        sprite.draw();
    }
}
