#pragma once
#include <memory>
#include "engine/render/shader_program.hpp"

inline std::unique_ptr<ShaderProgram> baseShader;
inline std::unique_ptr<ShaderProgram> lightingShader;
inline std::unique_ptr<ShaderProgram> uiShader;
