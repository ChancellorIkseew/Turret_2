#version 460 core
//
layout (location = 0) in vec2 inTexCoord;
layout (location = 1) in vec4 inColor;
//
layout (location = 0) out vec4 fragColor;
//
layout (binding = 0) uniform sampler2D gameTexture;
layout (binding = 1) uniform sampler2D lightMap;

void main() {
    vec4 texColor = texture(gameTexture, inTexCoord) * inColor;
    vec2 screenPos = gl_FragCoord.xy / textureSize(lightMap, 0);
    vec3 light = texture(lightMap, screenPos).rgb;
    
    vec3 ambient = vec3(1.0);
    vec3 finalLighting = ambient + light;
    fragColor = vec4(texColor.rgb * finalLighting, texColor.a);
}
