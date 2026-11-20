#version 330

in vec3 vertexPosition;
in vec4 vertexColor;

uniform mat4 mvp;
uniform vec4 massPoints[8];
uniform int massCount;

out vec3 fragWorldPosition;
out float fragCurvature;

void main()
{
    vec3 worldPosition = vertexPosition;
    float totalCurvature = 0.0;
    float fabricDepth = 0.0;

    for (int i = 0; i < 8; i++)
    {
        if (i >= massCount) break;

        vec2 delta = worldPosition.xz - massPoints[i].xz;
        float radius = length(delta);
        float strength = massPoints[i].w;

        float influence = strength / (radius * radius + 0.18);
        fabricDepth -= influence;
        totalCurvature += influence;
    }

    worldPosition.y += fabricDepth;
    fragWorldPosition = worldPosition;
    fragCurvature = clamp(totalCurvature, 0.0, 3.0);
    gl_Position = mvp * vec4(worldPosition, 1.0);
}