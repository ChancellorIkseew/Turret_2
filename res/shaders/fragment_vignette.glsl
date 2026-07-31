#version 460 core
//
layout (location = 0) in vec2 inTexCoord;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inLocalTex;
flat layout (location = 3) in vec2 inBlockSize; // screen size (1920.0x1080.0 or other)
//
layout (location = 0) out vec4 fragColor;
//
layout (binding = 0) uniform sampler2D gameTexture;
//
const float margin = 0.12;       // Distance to screen border (в 0..1)
const float cornerRadius = 0.15; //
const float softness = 0.25;     //
const float intensity = 0.85;    // (0.0 .. 1.0)
//
float sdRoundedBox(vec2 p, vec2 b, float r) {
    vec2 d = abs(p) - b + vec2(r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

void main() {
    vec2 size = max(inBlockSize, vec2(1.0));
    vec2 screenPos = gl_FragCoord.xy / size;

    vec2 uv = screenPos - vec2(0.5);
    float aspectRatio = size.x / size.y;
    uv.x *= aspectRatio;
    vec2 boxHalfSize = vec2(0.5 * aspectRatio - margin, 0.5 - margin);

    float distance = sdRoundedBox(uv, boxHalfSize, cornerRadius);
    float factor = smoothstep(0.0, softness, distance);
    float finalAlpha = factor * intensity * inColor.a;

    vec4 texColor = texture(gameTexture, inTexCoord);
    vec3 finalColor = inColor.rgb * texColor.rgb;

    fragColor = vec4(finalColor, finalAlpha);
}
