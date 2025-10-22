#pragma once
#include "game/physics/mob.hpp"

static inline Explosion explosion(0, 0, 0.0f);

static inline MobPreset cannonBoss("cannoner_bot", PixelCoord(22.5f, 28.0f), explosion, 0, 100, 1.0f, 17.0f);
static inline MobPreset gatlingBot("gatling_bot", PixelCoord(11.5f, 9.0f), explosion, 0, 50, 1.0f, 8.5f);
static inline MobPreset shuttle("shuttle", PixelCoord(13.5f, 13.5f), explosion, 0, 20, 8.0f, 10.5f);
