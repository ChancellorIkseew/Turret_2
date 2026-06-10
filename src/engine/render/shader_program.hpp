#pragma once
#include <string>
#include <iostream>
#include "glad/glad.h"

struct Pipeline {
    GLenum srcBlend = GL_SRC_ALPHA;
    GLenum dstBlend = GL_ONE_MINUS_SRC_ALPHA;
    bool useLightmap = false;
};

class ShaderProgram {
    Pipeline pipeline;
    unsigned int programID = 0;
public:
    ShaderProgram() = default;
    ShaderProgram(const char* vertexSource, const char* fragmentSource, const Pipeline pipeline) :
        pipeline(pipeline) {
        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        // Проверяем ошибки линковки
        int success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);
            std::cerr << "Shader Program Linking Error:\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~ShaderProgram() { glDeleteProgram(programID); }

    unsigned int getID() const { return programID; }
    Pipeline getPipeline() const { return pipeline; }
private:
    unsigned int compileShader(unsigned int type, const char* source) {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &success);
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "Shader Compilation Error (" << type << "):\n" << infoLog << std::endl;
        }
        return shader;
    }
};
