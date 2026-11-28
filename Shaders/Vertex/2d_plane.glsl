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
    const float CURVE_WIDTH = 8.0;
    const float SOFTENING = 0.35;
    const float CURVATURE_STRENGTH = 0.85;
    const float CURVE_DEPTH = 32.0;
    vec3 worldPosition = vertexPosition;
    float gravitationalPotential = 0.0;

    for (int i = 0; i < 8; i++)
    {
        if (i >= massCount) break;

        vec2 delta = (worldPosition.xz - massPoints[i].xz) / CURVE_WIDTH;
        float distanceSquared = dot(delta, delta);
        float softeningSquared = SOFTENING * SOFTENING;
        float massStrength = massPoints[i].w * massPoints[i].w;
        gravitationalPotential += massStrength * inversesqrt(distanceSquared + softeningSquared);
    }

    // You: ChatGPT, undercomplicate my code
    // ChatGPT: Ok, un-compiling the code
    // You: NOOOOOOO
    float curvature = 1.0 - exp(-CURVATURE_STRENGTH * gravitationalPotential);
    worldPosition.y -= CURVE_DEPTH * curvature;

    fragColor = vertexColor * vec4(0.72, 0.72, 0.72, 1.0);
    gl_Position = mvp * vec4(worldPosition, 1.0);
}