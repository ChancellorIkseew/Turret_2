#pragma once
#include "config.hpp"

typedef unsigned int GLenum;

struct Pipeline {
    GLenum srcBlend = 0x0302; // GL_SRC_ALPHA
    GLenum dstBlend = 0x0303; // GL_ONE_MINUS_SRC_ALPHA
    bool useLightmap = false;
};

class ShaderProgram {
    Pipeline pipeline;
    unsigned int programID = 0;
public:
    ShaderProgram(const char* vertexSource, const char* fragmentSource, const Pipeline pipeline);
    ~ShaderProgram();
    //
    unsigned int getID() const { return programID; }
    Pipeline getPipeline() const { return pipeline; }
private:
    t1_disable_copy_and_move(ShaderProgram)
};
