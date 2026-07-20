#pragma once

struct MobSoA;
class Presets;
class BlockMap;
class Schematic;
class BuildBeamsPool;
class BuiltInScripts;

namespace construction {
    void buildBlueprints(MobSoA& soa, const Presets& presets, Schematic& schematic,
        BlockMap& blocks, BuildBeamsPool& buildBeams);
}
