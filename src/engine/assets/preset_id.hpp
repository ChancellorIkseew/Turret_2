#pragma once
#include <cstdint>

namespace preset_tag {
    using PresetID = uint8_t;
    template<class Tag>
    struct StrongID {
        PresetID presetID;
        PresetID asUint() { return presetID; }
        auto operator<=>(const StrongID&) const = default;
        auto operator++(int) {
            StrongID old = *this;
            presetID++;
            return old;
        }
    };

    struct MobTag {};
    struct ShellTag {};
    struct TurretTag {};
}

using MobPresetID = preset_tag::StrongID<preset_tag::MobTag>;
using ShellPresetID = preset_tag::StrongID<preset_tag::ShellTag>;
using TurretPresetID = preset_tag::StrongID<preset_tag::TurretTag>;
