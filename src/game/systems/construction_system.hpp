#pragma once

struct MobSoA;
class Presets;
class SoundQueue;
class World;

namespace construction {
    void buildBlueprints(World& world, MobSoA& soa, const Presets& presets, SoundQueue& sounds);
}
