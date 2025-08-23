#include "load_content.hpp"
//
#include "engine/io/parser/list_parser.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/text.hpp"

static std::filesystem::path images("res/images");

void content::loadTextures() {
    Atlas::clear();
    list::Data textureNames = list::read("res/textures.list").value();
    for (const auto& name : textureNames) {
        Atlas::addTexture(images / name);
    }

    Atlas::addTexture("res/fonts/vc_latin.png");
    Atlas::addTexture("res/fonts/vc_cyrilic.png");
    Atlas::build();
    text::setFont("vc_latin", "vc_cyrilic");
}
