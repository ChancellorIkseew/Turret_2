#include "content.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/text.hpp"
#include "indexes.hpp"
#include "presets.hpp"

void content::load() {
    loadTextures();
    Indexes::load();
    Presets::load();
}

void content::loadTextures() {
    Atlas::clear();
    const auto fileNames = io::folders::getContents(io::folders::IMAGES, io::folders::ContentsType::file);
    for (const auto& fileName : fileNames) {
        Atlas::addTexture(io::folders::IMAGES / fileName);
    }

    Atlas::addTexture(io::folders::FONTS / "vc_latin.png");
    Atlas::addTexture(io::folders::FONTS / "vc_cyrilic.png");
    Atlas::addTexture(io::folders::FONTS / "t1_custom.png");
    Atlas::build();
    text::setFont("vc_latin", "vc_cyrilic", "t1_custom");
    text::loadCustomSymbols();
}
