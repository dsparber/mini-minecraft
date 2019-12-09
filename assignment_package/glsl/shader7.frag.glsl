#version 150

in vec2 fs_UV;
in vec4 fs_Pos;
uniform int u_Time;
out vec4 out_Col;
vec3 color;

uniform sampler2D u_RenderedTexture;
const float PI = 3.1415926535;
//fish eye effect
void main()
{

    const float PI = 3.1415926535;

    //float aperture = 178.0;
    float aperture = 250.0;
    float apertureHalf = 0.5 * aperture * (PI / 180.0);
    float maxFactor = sin(apertureHalf);

    vec2 uv;
    vec2 xy = 2.0 * fs_UV - 1.0;
    float d = length(xy);
    if (d < (2.0-maxFactor))
    {
        d = length(xy * maxFactor);
        float z = sqrt(1.0 - d * d);
        float r = atan(d, z) / PI;
        float phi = atan(xy.y, xy.x);

        uv.x = r * cos(phi) + 0.5;
        uv.y = r * sin(phi) + 0.5;
    }
    else
    {
        uv = fs_UV;
    }
    vec4 diffuseColor = texture(u_RenderedTexture, uv);
    out_Col = diffuseColor;
}

//void main()
//{
//    fs_UV *= .992;
//    vec4 diffuseColor = texture(u_RenderedTexture, 0.5 + fs_UV);
//  //vec4 diffuseColor = texture(u_RenderedTexture,0.5+(fs_UV.xy*=.992));
//  vec3 o = diffuseColor.rbb;
//  float z =  fs_Pos.z;
//  for (float i=0.;i<100.;i++)
//    z += pow(max(0.,.5-length(diffuseColor.rg)),2.)*exp(-i*.08);
//  out_Col=vec4(o*o+z,1);
//}
