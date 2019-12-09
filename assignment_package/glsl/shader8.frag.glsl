#version 150

in vec2 fs_UV;

uniform ivec2 u_Dimensions; // Screen dimensions
uniform vec3 u_Eye; // Camera pos
uniform int u_Time;
in vec4 fs_Pos;

out vec4 out_Col;
vec3 color;
uniform sampler2D u_RenderedTexture;

float rand(float n){return fract(sin(n) * 43758.5453123);}

float rand(vec2 n) {
        return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}


float noise(float p){
        float fl = floor(p);
  float fc = fract(p);
        return mix(rand(fl), rand(fl + 1.0), fc);
}

float noise(vec2 n) {
        const vec2 d = vec2(0.0, 1.0);
  vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
        return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

////sepia color
//void main()
//{
//    vec4 diffuseColor = texture(u_RenderedTexture, fs_UV);
//    vec3 c = vec3(diffuseColor.rgb);
//    //color = vec3(0.21 * c.r + 0.72 * c.g + 0.07 * c.b)*1.1;
//    //43.9% red, 25.9% green and 7.8% blue
//    //apply sepia effect
//    color = vec3(1.3 * c.r, 1.1 * c.g, 0.5 * c.b);
//    //vignette
//    //find the distance at which a fragment lies from the screen's center
//    float dist = sqrt(pow(fs_UV.x - 0.5, 2) + pow(fs_UV.y - 0.5, 2));
//    //larger distance, darker brightness.
//    color = (1 - dist * 1.2)  * color;


//    out_Col = vec4(color,1);
//}


//old movie
void main(){


vec2 pos = fs_UV;
//screen dither x：
vec2 nos_pos = pos - 0.5 + vec2(min(fract(u_Time) - 0.02, 0.0), 0.0);
vec2 abs_n_p = vec2(abs(nos_pos));
//screen dither y：
vec2 P = pos + vec2(0.0, max(min(40.0 * fract(u_Time) - 38.0, -40.0 * fract(u_Time) + 40.0), 0.0));
vec3 Col = texture(u_RenderedTexture, fs_UV).rgb;
//border x：
if(abs_n_p.x > 0.40)
{
    if(abs_n_p.x > 0.42 && abs_n_p.x < 0.48)
            Col = vec3(0.7) * step(mod(10.0 * (nos_pos.y + fract(u_Time)), 1.0), 0.8);
    else
        Col = vec3(0.0);
}
//border y：
if(abs_n_p.y > 0.40)
       Col *= mix(0.0, 1.0, 5.0 - 10.0 * abs_n_p.y);
//dark spots：
Col *= 4.0 * min(0.5 * noise(8.0 * nos_pos) + 0.5, 0.25);
//screen scratch：
float coord = rand(vec2(floor(4.0 * u_Time), pos.x));
if(abs(pos.x - coord) <= 0.001)
    Col *= 0.5 + 0.5 * noise(vec2(20.0, 2.0) * pos);
//light spots：
if(fract(0.1 * u_Time) > 0.9)
{
    Col += step(noise(6.0 * (pos + vec2(floor(4.0 * fract(u_Time)), 0.0))), -0.8);
    if(fract(0.1 * u_Time) > 0.98)
       Col *= 1.0 + 0.25 * abs(sin(20.0 * fract(2.0 * u_Time)));
}

//add a blc & white effect s
vec4 diffuseColor = texture(u_RenderedTexture, fs_UV);
vec3 c = vec3(diffuseColor.rgb);
color = vec3(0.21 * c.r + 0.72 * c.g + 0.07 * c.b)*1.1;

//vignette
//find the distance at which a fragment lies from the screen's center
float dist = sqrt(pow(fs_UV.x - 0.5, 2) + pow(fs_UV.y - 0.5, 2));
//larger distance, darker brightness.
color = (1 - dist * 1.2)  * color;

out_Col = vec4(Col* 0.5 + color * 0.5,1.0);
}
