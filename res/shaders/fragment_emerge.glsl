#version 460 core
//
layout (location = 0) in vec2 outTexCoords;
layout (location = 1) in vec4 outColor;
//
layout (location = 0) out vec4 finalScreenColor;
//
layout(binding = 0) uniform sampler2D textureAtlas;

float pseudoRandom(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    float progress = outColor.a;

    if (progress < 0.01) {
        progress = 0.05; 
    }

    // 1. Получаем точный размер атласа в пикселях (например, vec2(2048.0, 2048.0))
    vec2 atlasSize = vec2(textureSize(textureAtlas, 0));

    // 2. Находим точные координаты текущего пикселя внутри атласа
    vec2 pixelCoords = outTexCoords * atlasSize;

    // 3. Задаем размер зерна хаоса в текстурных пикселях.
    // floor(pixelCoords) — материализация пойдет строго по ОДНОМУ пикселю текстуры.
    // floor(pixelCoords / 2.0) — материализация пойдет блоками 2х2 текстурных пикселя.
    vec2 grainSize = floor(pixelCoords); 
    
    float noise = pseudoRandom(grainSize);

    if (noise > progress) {
        discard;
    }

    vec4 texColor = texture(textureAtlas, outTexCoords);
    
    if (noise > progress - 0.04) {
        texColor.rgb += vec3(0.1, 0.5, 1.0); 
    }

    finalScreenColor = vec4(texColor.rgb, texColor.a);
}
