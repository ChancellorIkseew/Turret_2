#version 460 core
//
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec4 inColor;
layout (location = 3) in vec2 inLocalTex; 
//
layout (location = 0) out vec2 outTexCoord;
layout (location = 1) out vec4 outColor;
layout (location = 2) out vec2 outLocalTex; 
flat layout (location = 3) out vec2 outBlockSize;
//
layout(std140, binding = 0) uniform CameraData {
    mat4 view;
};

void main() {
    outTexCoord = inTexCoord;
    outColor = inColor;
    outBlockSize = inLocalTex;
    outLocalTex = (inLocalTex + abs(inLocalTex)) * 0.5; // from [-size; size] to [0; size]
    gl_Position = view * vec4(inPosition, 0.0, 1.0);
}
