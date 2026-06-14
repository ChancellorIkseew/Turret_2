#version 460 core
//
layout (location = 0) in vec2 inTexCoord;
layout (location = 1) in vec4 inColor;
//
layout (location = 0) out vec4 fragColor;
//
layout (binding = 0) uniform sampler2D uiTexture;

void main() {
    vec4 texColor = texture(uiTexture, inTexCoord);
    if (texColor.a == 0.0) {
        discard;
    }
    fragColor = texColor * inColor;
}
