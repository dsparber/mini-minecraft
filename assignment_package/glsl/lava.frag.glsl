#version 150

in vec2 fs_UV;

out vec3 color;

uniform sampler2D u_RenderedTexture;
uniform int u_Time;
uniform ivec2 u_Dimensions;

void main()
{
    // Displacement
    vec2 displacement = vec2(0, 0);

    // Get color
    color = texture(u_RenderedTexture, fs_UV + displacement).rgb;

    // Add 30% red colored overlay
    color = .7 * color + .3 * vec3(1, .1, .1);
}
