#include "assets.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/render/glad/glad.h"
#include "engine/render/text.hpp"
#include "shaders.hpp"
#include "engine/io/io.hpp"

static void loadTextures(Atlas& atlas, Renderer& renderer) {
    atlas.clear();
    const auto fileNames = io::folders::getContents(io::folders::IMAGES, io::folders::ContentsType::file);
    for (const auto& fileName : fileNames) {
        atlas.addTexture(io::folders::IMAGES / fileName);
    }

    atlas.addTexture(io::folders::FONTS / "vc_latin.png");
    atlas.addTexture(io::folders::FONTS / "vc_cyrilic.png");
    atlas.build(renderer);
    text::setFont(atlas, "vc_latin", "vc_cyrilic");
}

static void loadSounds(Audio& audio) {
    //audio.clear();
    const auto fileNames = io::folders::getContents(io::folders::SOUNDS, io::folders::ContentsType::file);
    for (const auto& fileName : fileNames) {
        audio.loadSound(io::folders::trimExtensions(fileName), io::folders::SOUNDS / fileName);
    }
}

void Assets::load(Renderer& renderer) {
    loadTextures(atlas, renderer);
    loadSounds(audio);
    indexes.load();
    presets.load(atlas);
    waves.load();

    auto path = io::folders::RES / "shaders";

    std::string vertexBase = io::readFile(path / "vertex.glsl");
    std::string fragmentBase = io::readFile(path / "fragment_texturing.glsl");
    shaders.baseShader.emplace(vertexBase.c_str(), fragmentBase.c_str(), Pipeline());

    std::string vertexLighting = io::readFile(path / "vertex_lighting.glsl");
    std::string fragmentLighting = io::readFile(path / "fragment_lighting.glsl");
    shaders.lightingShader.emplace(vertexLighting.c_str(), fragmentLighting.c_str(), Pipeline(GL_ONE, GL_ONE, true));

    std::string vertexEmerge = io::readFile(path / "vertex_emerge.glsl");
    std::string fragmentEmerge = io::readFile(path / "fragment_emerge.glsl");
    shaders.emergeShader.emplace(vertexEmerge.c_str(), fragmentEmerge.c_str(), Pipeline());

    std::string vertexUI = io::readFile(path / "vertex_ui.glsl");
    std::string fragmentUI = io::readFile(path / "fragment_ui.glsl");
    shaders.uiShader.emplace(vertexUI.c_str(), fragmentUI.c_str(), Pipeline());
}
