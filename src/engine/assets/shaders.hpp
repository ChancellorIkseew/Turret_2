#pragma once
#include <optional>
#include "engine/render/shader_program.hpp"

class Shaders {
public:
    std::optional<ShaderProgram> monochrome;
    std::optional<ShaderProgram> base;
    std::optional<ShaderProgram> lighting;
    std::optional<ShaderProgram> additiveLight;
    std::optional<ShaderProgram> smoke;
    std::optional<ShaderProgram> squareShadow;
    std::optional<ShaderProgram> emerge;
    std::optional<ShaderProgram> spark;
    std::optional<ShaderProgram> casing;
    std::optional<ShaderProgram> shield;
    std::optional<ShaderProgram> buildBeam;
    std::optional<ShaderProgram> vignette;
    std::optional<ShaderProgram> ui;
};
