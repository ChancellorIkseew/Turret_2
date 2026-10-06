#include "assets.hpp"
//
#include "engine/io/folders.hpp"
#include "engine/render/glad/glad.h"
#include "engine/render/text.hpp"

static void loadTextures(Atlas& atlas, Renderer& renderer) {
    atlas.clear();
    for (const auto& entry : fs::recursive_directory_iterator(io::folders::IMAGES)) {
        atlas.addTexture(entry.path());
    }

    atlas.addTexture(io::folders::FONTS / "vc_latin.png");
    atlas.addTexture(io::folders::FONTS / "vc_cyrilic.png");
    atlas.build(renderer);
    text::setFont(atlas, "vc_latin", "vc_cyrilic");
}

static void loadSounds(Audio& audio) {
    //audio.clear();
    for (const auto& entry : fs::recursive_directory_iterator(io::folders::SOUNDS)) {
        audio.loadSound(entry.path());
    }
    for (const auto& entry : fs::recursive_directory_iterator(io::folders::RES / "music")) {
        audio.loadSound(entry.path());
    }
}

void Assets::load(Renderer& renderer) {
    loadTextures(atlas, renderer);
    loadSounds(audio);
    indexes.load();
    presets.load(atlas);
    waves.load();

    auto path = io::folders::RES / "shaders";
    shaders.monochrome.emplace(path / "vertex.glsl", path / "fragment_monochrome.glsl", Pipeline());
    shaders.base.emplace(path / "vertex.glsl", path / "fragment_texturing.glsl", Pipeline());
    shaders.lighting.emplace(path / "vertex_lighting.glsl", path / "fragment_lighting.glsl", Pipeline(GL_ONE, GL_ONE, true));
    shaders.additiveLight.emplace(path / "vertex_lighting.glsl", path / "fragment_lighting.glsl", Pipeline(GL_ONE, GL_ONE, false));
    shaders.smoke.emplace(path / "vertex_lighting.glsl", path / "fragment_smoke.glsl", Pipeline());
    shaders.squareShadow.emplace(path / "vertex_lighting.glsl", path / "fragment_shadow.glsl", Pipeline());
    shaders.emerge.emplace(path / "vertex.glsl", path / "fragment_emerge.glsl", Pipeline());
    shaders.spark.emplace(path / "vertex.glsl", path / "fragment_texturing.glsl", Pipeline(GL_SRC_ALPHA, GL_ONE, false));
    shaders.casing.emplace(path / "vertex.glsl", path / "fragment_casing.glsl", Pipeline(GL_ONE, GL_ONE_MINUS_SRC_COLOR, false));
    shaders.shield.emplace(path / "vertex.glsl", path / "fragment_shield.glsl", Pipeline());
    shaders.buildBeam.emplace(path / "vertex.glsl", path / "fragment_build_beam.glsl", Pipeline());
    shaders.vignette.emplace(path / "vertex.glsl", path / "fragment_vignette.glsl", Pipeline());
    shaders.ui.emplace(path / "vertex.glsl", path / "fragment_ui.glsl", Pipeline());
}
