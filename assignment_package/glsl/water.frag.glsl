#version 150

uniform ivec2 u_Dimensions;
uniform int u_Time;

in vec2 fs_UV;

out vec3 color;

uniform sampler2D u_RenderedTexture;

void main()
{
    // Displacement
    vec2 displacement = vec2(0, 0);

    // Get color
    color = texture(u_RenderedTexture, fs_UV + displacement).rgb;

    // Add 30% blue color overlay
    color = .7 * color + .3 * vec3(0, .4, 1);
}
