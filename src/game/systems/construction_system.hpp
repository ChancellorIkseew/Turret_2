#pragma once

struct MobSoA;
class Presets;
class Blueprints;
class BuiltInScripts;

namespace construction {
    void buildBlueprints(MobSoA& soa, const Presets& presets, Blueprints& blueprints, BuiltInScripts& scripts);
}
