#pragma once
#include "engine/render/atlas.hpp"
#include "engine/audio/audio.hpp"
#include "indexes.hpp"
#include "presets.hpp"

class Renderer;

class Assets {
    Atlas atlas;
    Audio audio;
    Presets presets;
    Indexes indexes;
public:
    Assets() = default;
    void load(Renderer& renderer);
    //
    const Atlas&   getAtlas()   const noexcept { return atlas; }
    const Audio&   getAudio()   const noexcept { return audio; }
    const Presets& getPresets() const noexcept { return presets; }
    const Indexes& getIndexes() const noexcept { return indexes; }
    //
    Audio& getAudio() noexcept { return audio; }
};
