#pragma once
#include <optional>
#include "engine/render/shader_program.hpp"

class Shaders {
public:
    std::optional<ShaderProgram> baseShader;
    std::optional<ShaderProgram> lightingShader;
    std::optional<ShaderProgram> emergeShader;
    std::optional<ShaderProgram> shieldShader;
    std::optional<ShaderProgram> uiShader;
};
