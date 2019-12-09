#version 150
uniform int u_Time;
in vec2 fs_UV;
out vec4 out_Col;
vec3 color;
uniform ivec2 u_Dimensions;
uniform sampler2D u_RenderedTexture;
//horror movie effect
float PI = 3.14159265358979323846;
in vec4 fs_Pos;

vec2 random2(vec2 p) {
    vec2 point = fract(sin(vec2(dot(p, vec2(127.1, 311.7)),
                          dot(p, vec2(269.5,183.3))))
                 * 43758.5453);
    // Animate the point
    point = 0.5 + 0.5 * sin(u_Time * 0.01 + 6.2831 * point); // 0 to 1 range
    return point;
}

vec3 random3(vec3 c) {
        float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
        vec3 r;
        r.z = fract(512.0*j);
        j *= .125;
        r.x = fract(512.0*j);
        j *= .125;
        r.y = fract(512.0*j);
        return r-0.5;
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
    vec4 diffuseColor = texture(u_RenderedTexture, fs_UV);
    vec3 c = vec3(diffuseColor.rgb);
    color = vec3(0.21 * c.r + 0.72 * c.g + 0.07 * c.b)*1.1;

    vec2 rand2Dist = random2(fs_UV);
    //vignette
    //find the distance at which a fragment lies from the screen's center
    float dist1 = sqrt(pow(fs_UV.x - 0.5, 2) + pow(fs_UV.y - 0.5, 2));
    float dist2 = sqrt(pow(fs_UV.x - 0.6, 2) + pow(fs_UV.y - 0.4, 2));
    float dist = mix(dist1,dist2,sin(u_Time/10.0));
    //larger distance, darker brightness.
    color = (1 - dist * 1.8)  * color;
    //out_Col = vec4(WorleyNoise(5,fs_UV));
    out_Col = vec4(color,1);
    vec3 noiseCol = random3(vec3(fs_Pos));

    out_Col = vec4(mix(color,noiseCol,sin(u_Time)/20.0),1);
    //float threshold1  = WorleyNoise(3,fs_UV);
    //float threshold2  = WorleyNoise(2,fs_UV);
    //float th = mix(threshold1,threshold2,sin(u_Time/20.0));
    //out_Col = vec4(color-(th*(1-th)/2),1);

}
