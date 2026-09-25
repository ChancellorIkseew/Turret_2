#version 460 core
//
layout (location = 0) in vec2 inTexCoord;
layout (location = 1) in vec4 inColor;
//
layout (location = 0) out vec4 fragColor;
//
layout (binding = 0) uniform sampler2D gameTexture;

void main() {
    vec4 texColor = texture(gameTexture, inTexCoord) * inColor;
    float modifier = 1.0 - texColor.a;
    float r = texColor.r + (1.0 - texColor.r) * modifier;
    float g = texColor.g + (1.0 - texColor.g) * modifier;
    float b = texColor.b + (1.0 - texColor.b) * modifier;
    fragColor = vec4(r, g, b, texColor.a);
}
