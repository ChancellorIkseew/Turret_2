#version 460 core

layout (location = 0) in vec2 outLocalPos; // [-1, 1]
layout (location = 1) in vec4 inColor;     // RGB = color, A = intensity

layout (location = 0) out vec4 fragColor;

void main() {
    float distance = length(outLocalPos);
    
    // Используем ровно тот же smoothstep, что и в шейдере освещения
    float edgeSmoothing = smoothstep(1.0, 0.0, distance);
    
    // Вся интенсивность затухания идет в альфа-канал (прозрачность)
    float finalAlpha = inColor.a * edgeSmoothing;
    
    // fragColor выводит цвет (inColor.rgb) и альфа-прозрачность
    fragColor = vec4(inColor.rgb, finalAlpha);
}
