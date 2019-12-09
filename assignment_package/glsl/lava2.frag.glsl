#version 150

uniform ivec2 u_Dimensions;
uniform int u_Time;

in vec2 fs_UV;

out vec4 color;

uniform sampler2D u_RenderedTexture;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)),
                 dot(p, vec2(269.5,183.3))))
                 * 43758.5453);
}

float WorleyNoise(float size, vec2 uv) {
    uv *= size; // Now the space is 10x10 instead of 1x1. Change this to any number you want.
    vec2 uvInt = floor(uv);
    vec2 uvFract = fract(uv);
    float minDist = 1.0; // Minimum distance initialized to max.
    for(int y = -1; y <= 1; ++y) {
        for(int x = -1; x <= 1; ++x) {
            vec2 neighbor = vec2(float(x), float(y)); // Direction in which neighbor cell lies
            vec2 point = random2(uvInt + neighbor); // Get the Voronoi centerpoint for the neighboring cell
            vec2 diff = neighbor + point - uvFract; // Distance between fragment coord and neighbor’s Voronoi point
            float dist = length(diff);
            minDist = min(minDist, dist);
        }
    }
    return minDist;
}

void main()
{
    // Displacement
    vec2 displacement = vec2(0, 0);
    vec4 origC = texture(u_RenderedTexture, fs_UV);
    // Add 30% blue color overlay
    origC = .7 * origC + .3 * vec4(0, .4, 1, 1 / .3);

    float tPos = mix(WorleyNoise(-1.0, fs_UV), WorleyNoise(5.0, fs_UV), cos(u_Time/10));
    vec4 noiseC = texture(u_RenderedTexture, fs_UV + tPos / 15.0);

    // Get color
    color = .7 * noiseC + .3 * vec4(1, .1, .1, 1 / .3);
    }
