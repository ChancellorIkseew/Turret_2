#pragma once

struct MobSoA;
class BlockMap;
class BuildBeamsPool;
class ParticlesPool;
class Presets;
class Schematic;
class SoundQueue;
class TeamsPool;

namespace construction {
    void buildBlueprints(MobSoA& soa, const Presets& presets, Schematic& schematic,
        BlockMap& blocks, BuildBeamsPool& buildBeams, TeamsPool& teams, SoundQueue& sounds, ParticlesPool& particles);
}
