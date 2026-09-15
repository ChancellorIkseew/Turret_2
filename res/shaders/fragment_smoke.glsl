#version 460 core
//
layout (location = 0) in vec2 outLocalPos; // [-1, 1]
layout (location = 1) in vec4 inColor;     // RGB = color, A = intensity
//
layout (location = 0) out vec4 fragColor;

void main() {
    float distance = length(outLocalPos);
    float edgeSmoothing = smoothstep(1.0, 0.0, distance);
    float finalAlpha = inColor.a * edgeSmoothing;
    fragColor = vec4(inColor.rgb, finalAlpha);
}
