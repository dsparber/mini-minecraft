#version 150

uniform ivec2 u_Dimensions;
uniform int u_Time;

in vec2 fs_UV;
in vec4 fs_Pos;

out vec4 color;

uniform sampler2D u_RenderedTexture;
//painter/kaledoscope
vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)),
                 dot(p, vec2(269.5,183.3))))
                 * u_Time * 0.02);
}


vec2 WorleyNoise(vec2 uv, float n) {
    uv *= n;
    vec2 uvInt = floor(uv);
    vec2 uvFract = fract(uv);
    float minDist = 1.0; // Minimum distance initialized to max.
    vec2 minVector = vec2(0); // Vector to min center
    for(int y = -1; y <= 1; ++y) {
        for(int x = -1; x <= 1; ++x) {
            vec2 neighbor = vec2(float(x), float(y)); // Direction in which neighbor cell lies
            vec2 point = random2(uvInt + neighbor); // Get the Voronoi centerpoint for the neighboring cell
            vec2 diff = neighbor + point - uvFract; // Distance between fragment coord and neighbor’s Voronoi point
            float dist = length(diff);
            if (dist < minDist) {
                minVector = neighbor + point - uvFract;
                minDist = dist;
            }
        }
    }
    // Return vector to nearest center
    return minVector / n;
}


void main()
{
    // Amount of center points n x n
    int n1 = 5;
    int n2 = 1;
    //float n = mix (n1,n2,sin(u_Time/20.0));
    float n = 3.0;
    // Nearest center
    vec2 center = WorleyNoise(fs_UV, n);

    // Displacement
    vec2 displacement = center * 0.2f;

    // Get color
    color = texture(u_RenderedTexture, fs_UV + displacement);

    //color = 1 * color + .2 * vec4(0, .4, 1, 1 / .3);
    color = clamp(color*1.5, 0.2, 0.9 );

}


