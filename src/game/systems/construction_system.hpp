#pragma once

struct MobSoA;
class BlockMap;
class BuildBeamsPool;
class Presets;
class Schematic;
class TeamsPool;


namespace construction {
    void buildBlueprints(MobSoA& soa, const Presets& presets, Schematic& schematic,
        BlockMap& blocks, BuildBeamsPool& buildBeams, TeamsPool& teams);
}
