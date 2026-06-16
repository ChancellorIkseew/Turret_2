#version 460 core
//
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inColor;
//
layout (location = 0) out vec2 outTexCoord;
layout (location = 1) out vec4 outColor;
//
layout(std140, binding = 0) uniform CameraData {
    mat4 viewMatrix;
};

void main() {
    outTexCoord = inTexCoord;
    outColor = inColor;
    gl_Position = viewMatrix * vec4(inPosition, 0.0, 1.0);
}
