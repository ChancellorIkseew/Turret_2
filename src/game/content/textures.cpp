#include "content.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/renderer.hpp"
#include "engine/render/text.hpp"
#include "indexes.hpp"
#include "presets.hpp"

void content::load(Atlas& atlas, Renderer& renderer) {
    loadTextures(atlas, renderer);
    Indexes::load();
    Presets::load(atlas);
}

void content::loadTextures(Atlas& atlas, Renderer& renderer) {
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
