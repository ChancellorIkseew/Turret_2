#pragma once
#include "config.hpp"

class Texture2D {
    unsigned int textureID = 0;
public:
    Texture2D(int width, int height, const unsigned char* data);
    ~Texture2D();
    //
    unsigned int getID() const { return textureID; }
private:
    t1_disable_copy_and_move(Texture2D)
};
