#pragma once

struct MobSoA;
class BlockMap;
class BuildBeamsPool;
class Presets;
class Schematic;
class TeamsPool;
class SoundQueue;

namespace construction {
    void buildBlueprints(MobSoA& soa, const Presets& presets, Schematic& schematic,
        BlockMap& blocks, BuildBeamsPool& buildBeams, TeamsPool& teams, SoundQueue& sounds);
}
