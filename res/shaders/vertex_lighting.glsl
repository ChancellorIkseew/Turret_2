#version 460 core
//
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inColor;
//
layout (location = 0) out vec2 outLocalPos; // [-1, 1]
layout (location = 1) out vec4 outColor;
//
layout(std140, binding = 0) uniform CameraData {
    mat4 view;
};

void main() {
    outColor = inColor;
    outLocalPos = inTexCoord * 2.0 - vec2(1.0);
    gl_Position = view * vec4(inPosition, 0.0, 1.0);
}
