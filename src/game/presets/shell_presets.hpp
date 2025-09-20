#pragma once
#include "game/physics/shell.hpp"

static inline Explosion cannonShellExplosion(0, 0, 0.0f);

static inline ShellPreset cannonShell("cannon_shell", PixelCoord(1.5f, 3.0f), cannonShellExplosion, 15, 180, 3.0f);
