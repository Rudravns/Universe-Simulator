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

    // You: ChatGPT, undercomplicate my code
    // ChatGPT: Ok, un-compiling the code
    // You: NOOOOOOO
    float curvature = 1.0 - exp(-0.55 * gravitationalPotential);
    worldPosition.y -= 8.0 * curvature;

    fragColor = vertexColor * vec4(0.72, 0.72, 0.72, 1.0);
    gl_Position = mvp * vec4(worldPosition, 1.0);
}