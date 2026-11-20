#version 330

in vec3 fragWorldPosition;
in float fragCurvature;

out vec4 finalColor;

void main()
{
    vec2 gridPosition = fragWorldPosition.xz;
    vec2 gridLine = abs(fract(gridPosition + 0.5) - 0.5) / 0.5;
    float line = 1.0 - smoothstep(0.88, 1.0, min(gridLine.x, gridLine.y));

    vec3 deepBlue = vec3(0.02, 0.09, 0.18);
    vec3 curvedPurple = vec3(0.30, 0.12, 0.55);
    vec3 base = mix(deepBlue, curvedPurple, fragCurvature / 3.0);
    base = mix(base, vec3(0.50, 0.75, 1.00), line * 0.48);

    finalColor = vec4(base, 0.90);
}
