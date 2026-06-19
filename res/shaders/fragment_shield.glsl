#version 460 core
//
layout (location = 0) in vec2 inTexCoord;
layout (location = 1) in vec4 inColor;
//
layout (location = 0) out vec4 fragColor;
//
layout (binding = 1) uniform sampler2D lightMap;

void main() {
    vec3 shieldColor = vec3(0.3, 0.38, 0.5);
    
    vec2 uv = inTexCoord * 2.0 - 1.0;
    float distance = length(uv);
    
    float edgeSmoothing = 0.015;
    float edgeMask = smoothstep(1.0, 1.0 - edgeSmoothing, distance);
    if (edgeMask < 0.01)
        discard;

    float fresnel = pow(distance, 3.0);
    float alpha = mix(0.1, 0.85, fresnel) * edgeMask;

    vec2 screenPos = gl_FragCoord.xy / textureSize(lightMap, 0);
    vec3 light = texture(lightMap, screenPos).rgb;
    
    vec3 ambient = vec3(1.0);
    vec3 finalLighting = ambient + light;
    vec3 litShieldColor = shieldColor * finalLighting;

    vec3 finalColor = litShieldColor + (shieldColor * fresnel * 0.6);
    
    fragColor = vec4(finalColor, alpha);
}
