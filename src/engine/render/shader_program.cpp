#include "shader_program.hpp"
//
#include "engine/debug/logger.hpp"
#include "glad/glad.h"

static debug::Logger logger("shaders");

static unsigned int compileShader(unsigned int type, const char* source) {
    const unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &success);
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        logger.error() << "Shader Compilation Error (" << type << "):\n" << infoLog;
    }
    return shader;
}

ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource, const Pipeline pipeline) :
    pipeline(pipeline) {
    const unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        logger.error() << "Shader Program Linking Error:\n" << infoLog;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);
}
