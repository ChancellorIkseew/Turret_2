#version 460 core
//
layout (location = 0) in vec2 outTexCoords;
layout (location = 1) in vec4 outColor;
layout (location = 2) in vec2 outLocalTex;  // x = WorldX, y = WorldY
flat layout (location = 3) in vec2 outBlockSize; 
//
layout (location = 0) out vec4 finalScreenColor;
//
layout(binding = 0) uniform sampler2D textureAtlas;

void main() {
    float lineWidth = 4.0;
    float period = 16.0;

    float worldX = outLocalTex.x;
    float worldY = outLocalTex.y;

    float diagonalPos = (worldX - worldY) * 0.70710678;

    float ticks = outColor.a * 255.0;
    float movement = ticks * 0.5;

    float movingDiag = diagonalPos - movement;

    float lineMask = step(mod(movingDiag, period), lineWidth);

    float beamBody = 0.15;      
    float lineBright = 0.4;   
    
    float finalAlpha = mix(beamBody, lineBright, lineMask);

    vec4 texColor = texture(textureAtlas, outTexCoords);
    finalScreenColor = vec4(outColor.rgb, finalAlpha * texColor.a);
}
