#include "content.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/parser/list_parser.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/text.hpp"
#include "indexes.hpp"

void content::load() {
    loadTextures();
    Indexes::load();
}

void content::loadTextures() {
    Atlas::clear();
    list::Data textureNames = list::read(io::folders::CONTENT / "textures.list");
    for (const auto& name : textureNames) {
        Atlas::addTexture(io::folders::IMAGES / name);
    }

    Atlas::addTexture(io::folders::FONTS / "vc_latin.png");
    Atlas::addTexture(io::folders::FONTS / "vc_cyrilic.png");
    Atlas::addTexture(io::folders::FONTS / "t1_custom.png");
    Atlas::build();
    text::setFont("vc_latin", "vc_cyrilic", "t1_custom");
    text::loadCustomSymbols();
}
