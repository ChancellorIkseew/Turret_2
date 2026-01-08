#pragma once
#include "engine/io/folders.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/text.hpp"
#include "engine/audio/audio.hpp"
#include "game/content/indexes.hpp"
#include "game/content/presets.hpp"

class Assets {


public:
    void loadTextures(Atlas& atlas) {
        atlas.clear();
        const auto fileNames = io::folders::getContents(io::folders::IMAGES, io::folders::ContentsType::file);
        for (const auto& fileName : fileNames) {
            atlas.addTexture(io::folders::IMAGES / fileName);
        }
        atlas.build();
    }

    void loadSounds(Audio& audio) {
        const auto fileNames = io::folders::getContents(io::folders::SOUNDS, io::folders::ContentsType::file);
        for (const auto& fileName : fileNames) {
            //audio.clear();
            audio.loadSound(io::folders::trimExtensions(fileName), io::folders::SOUNDS / fileName);
        }
    }

    void load(Atlas& atlas, Audio& audio, content::Indexes& indexes, content::Presets& presets) {
        loadTextures(atlas);
        loadSounds(audio);

        indexes.load();
        presets.load();
    }


};
