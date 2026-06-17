#include "shader_program.hpp"
//
#include "engine/debug/logger.hpp"
#include "engine/io/io.hpp"
#include "glad/glad.h"

static debug::Logger logger("shaders");

static unsigned int compileShader(unsigned int type, const fs::path path) {
    std::string source = io::readFile(path, io::Log::only_error);
    const char* sourcePtr = source.c_str();

    const unsigned int shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &sourcePtr, nullptr);
    glCompileShader(shaderID);

    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &success);
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        logger.error() << "Shader Compilation Error (" << path << "):\n" << infoLog;
    }
    return shaderID;
}

ShaderProgram::ShaderProgram(const fs::path vertexPath, const fs::path fragmentPath, const Pipeline pipeline) :
    pipeline(pipeline) {
    const unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexPath);
    const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        logger.error() << "Shader Program Linking Error: ("  << vertexPath << " + " << fragmentPath << ")\n" << infoLog;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);
}
