#pragma once

struct MobSoA;
class Presets;
class BlockMap;
class Blueprints;
class BuildBeamsPool;
class BuiltInScripts;

namespace construction {
    void buildBlueprints(MobSoA& soa, const Presets& presets, Blueprints& blueprints,
        BlockMap& blocks, BuildBeamsPool& buildBeams);
}
