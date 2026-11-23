#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;

uniform mat4 mvp;
uniform vec4 massPoints[8];
uniform int massCount;

out vec2 fragTexCoord;
out vec4 fragColor;

void main()
{
    vec3 worldPosition = vertexPosition;
    float gravitationalPotential = 0.0;

    for (int i = 0; i < 8; i++)
    {
        if (i >= massCount) break;

        vec2 delta = worldPosition.xz - massPoints[i].xz;
        float distanceSquared = dot(delta, delta);
        float softeningSquared = 0.35 * 0.35;
        gravitationalPotential += massPoints[i].w * inversesqrt(distanceSquared + softeningSquared);
    }

    float curvature = 1.0 - exp(-0.55 * gravitationalPotential);
    worldPosition.y -= 8.0 * curvature;

    vec2 gridDistance = abs(fract(vertexPosition.xz + 0.5) - 0.5);
    float gridLine = 1.0 - smoothstep(0.40, 0.48, min(gridDistance.x, gridDistance.y));

    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor * vec4(vec3(0.72), gridLine);
    gl_Position = mvp * vec4(worldPosition, 1.0);
}