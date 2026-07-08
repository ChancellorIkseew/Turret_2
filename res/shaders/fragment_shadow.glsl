#version 460 core
//
layout (location = 0) in vec2 outLocalPos; // [-1, 1]
layout (location = 1) in vec4 inColor;
//
layout (location = 0) out vec4 fragColor;

void main() {
    vec2 distance = abs(outLocalPos);
    float fadeX = smoothstep(1.0, 0.1, distance.x);
    float fadeY = smoothstep(1.0, 0.1, distance.y);
    float edgeSmoothing = fadeX * fadeY;
    fragColor = vec4(0.0, 0.0, 0.0, 0.7 * edgeSmoothing);
}
