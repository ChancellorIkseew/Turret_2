#pragma once
#include "engine/render/atlas.hpp"
#include "engine/audio/audio.hpp"
#include "indexes.hpp"
#include "presets.hpp"
#include "waves.hpp"

class Renderer;

class Assets {
    Atlas atlas;
    Audio audio;
    Presets presets;
    Indexes indexes;
    Waves waves;
public:
    Assets() = default;
    void load(Renderer& renderer);
    //
    const Atlas&   getAtlas()   const noexcept { return atlas; }
    const Audio&   getAudio()   const noexcept { return audio; }
    const Presets& getPresets() const noexcept { return presets; }
    const Indexes& getIndexes() const noexcept { return indexes; }
    const Waves&   getWaves()   const noexcept { return waves; }
    //
    Audio& getAudio() noexcept { return audio; }
};
