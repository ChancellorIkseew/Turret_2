#version 460 core
//
layout (location = 0) in vec2 outTexCoords;
layout (location = 1) in vec4 outColor;
layout (location = 2) in vec2 outLocalTex;
flat layout (location = 3) in vec2 outBlockSize;
//
layout (location = 0) out vec4 finalScreenColor;
//
layout(binding = 0) uniform sampler2D textureAtlas;

void main() {
    float progress = outColor.a;
    vec2 localPixel = floor(outLocalTex);
    vec2 center = (abs(outBlockSize) - vec2(1.0)) * 0.5;
    float distanceToCenter = abs(localPixel.x - center.x) + abs(localPixel.y - center.y);
    float maxDistance = (center.x + center.y) * 2.0;
    float waveValue = distanceToCenter / maxDistance;
    
    bool onBorder = localPixel.x == 0.0 || localPixel.y == 0.0 || 
        localPixel.x >= abs(outBlockSize.x) - 1.0 || 
        localPixel.y >= abs(outBlockSize.y) - 1.0;

    float threshold = 1.0 - progress;
    if (!onBorder && waveValue < threshold)
        discard;
    vec4 texColor = texture(textureAtlas, outTexCoords);
    if (onBorder || waveValue < threshold + 0.04)
        texColor = vec4(outColor.rgb, 0.6);

    finalScreenColor = texColor;
}
