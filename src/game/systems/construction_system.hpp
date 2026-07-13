#pragma once

struct MobSoA;
class Presets;
class BlockMap;
class Blueprints;
class BuiltInScripts;

namespace construction {
    void buildBlueprints(MobSoA& soa, const Presets& presets, Blueprints& blueprints, BuiltInScripts& scripts, BlockMap& blocks);
}
