#include "load_content.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/io/parser/list_parser.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/text.hpp"

void content::loadTextures() {
    Atlas::clear();
    list::Data textureNames = list::read(io::folders::RES / "textures.list");
    for (const auto& name : textureNames) {
        Atlas::addTexture(io::folders::IMAGES / name);
    }

    Atlas::addTexture(io::folders::FONTS / "vc_latin.png");
    Atlas::addTexture(io::folders::FONTS / "vc_cyrilic.png");
    Atlas::build();
    text::setFont("vc_latin", "vc_cyrilic");
}
