#pragma once
#include <compare>
#include <cstdint>

namespace preset_tag {
    using PresetID = uint8_t;
    template<class Tag>
    struct StrongID {
        PresetID presetID;
        PresetID asUint() const { return presetID; }
        auto operator<=>(const StrongID&) const = default;
        auto operator++(int) {
            StrongID old = *this;
            presetID++;
            return old;
        }
    };

    struct BlockTag {};
    struct ItemTag {};
    struct MobTag {};
    struct ShellTag {};
    struct TurretTag {};
}

using BlockPresetID  = preset_tag::StrongID<preset_tag::BlockTag>;
using ItemPresetID   = preset_tag::StrongID<preset_tag::ItemTag>;
using MobPresetID    = preset_tag::StrongID<preset_tag::MobTag>;
using ShellPresetID  = preset_tag::StrongID<preset_tag::ShellTag>;
using TurretPresetID = preset_tag::StrongID<preset_tag::TurretTag>;
