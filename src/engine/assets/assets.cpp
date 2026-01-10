#include "assets.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/render/text.hpp"

static void loadTextures(Atlas& atlas, Renderer& renderer) {
    atlas.clear();
    const auto fileNames = io::folders::getContents(io::folders::IMAGES, io::folders::ContentsType::file);
    for (const auto& fileName : fileNames) {
        atlas.addTexture(io::folders::IMAGES / fileName);
    }

    atlas.addTexture(io::folders::FONTS / "vc_latin.png");
    atlas.addTexture(io::folders::FONTS / "vc_cyrilic.png");
    atlas.addTexture(io::folders::FONTS / "t1_custom.png");
    atlas.build(renderer);
    text::setFont("vc_latin", "vc_cyrilic", "t1_custom");
    text::loadCustomSymbols();
}

static void loadSounds(Audio& audio) {
    //audio.clear();
    const auto fileNames = io::folders::getContents(io::folders::SOUNDS, io::folders::ContentsType::file);
    for (const auto& fileName : fileNames) {
        audio.loadSound(io::folders::trimExtensions(fileName), io::folders::SOUNDS / fileName);
    }
}

static void loadPresets(Atlas& atlas, Audio& audio, Presets& presets) {
    
}

void Assets::load(Renderer& renderer) {
    loadTextures(atlas, renderer);
    loadSounds(audio);
    indexes.load();
    presets.load(atlas);
}
