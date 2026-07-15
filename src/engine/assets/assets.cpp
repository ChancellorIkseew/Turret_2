#include "assets.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/render/glad/glad.h"
#include "engine/render/text.hpp"

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
    shaders.baseShader.emplace(path / "vertex.glsl", path / "fragment_texturing.glsl", Pipeline());
    shaders.lightingShader.emplace(path / "vertex_lighting.glsl", path / "fragment_lighting.glsl", Pipeline(GL_ONE, GL_ONE, true));
    shaders.additiveLightShader.emplace(path / "vertex_lighting.glsl", path / "fragment_lighting.glsl", Pipeline(GL_ONE, GL_ONE, false));
    shaders.squareShadowShader.emplace(path / "vertex_lighting.glsl", path / "fragment_shadow.glsl", Pipeline(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, false));
    shaders.emergeShader.emplace(path / "vertex.glsl", path / "fragment_emerge.glsl", Pipeline());
    shaders.shieldShader.emplace(path / "vertex.glsl", path / "fragment_shield.glsl", Pipeline());
    shaders.buildBeamShader.emplace(path / "vertex.glsl", path / "fragment_build_beam.glsl", Pipeline());
    shaders.uiShader.emplace(path / "vertex.glsl", path / "fragment_ui.glsl", Pipeline());
}
