#version 150

in vec2 fs_UV;
in vec4 fs_Pos;
uniform int u_Time;
out vec4 out_Col;
vec3 color;

uniform sampler2D u_RenderedTexture;
const float PI = 3.1415926535;

//void main()
//{

//    const float PI = 3.1415926535;

//    //float aperture = 178.0;
//    float aperture = 250.0;
//    float apertureHalf = 0.5 * aperture * (PI / 180.0);
//    float maxFactor = sin(apertureHalf);

//    vec2 uv;
//    vec2 xy = 2.0 * fs_UV - 1.0;
//    float d = length(xy);
//    if (d < (2.0-maxFactor))
//    {
//        d = length(xy * maxFactor);
//        float z = sqrt(1.0 - d * d);
//        float r = atan(d, z) / PI;
//        float phi = atan(xy.y, xy.x);

//        uv.x = r * cos(phi) + 0.5;
//        uv.y = r * sin(phi) + 0.5;
//    }
//    else
//    {
//        uv = fs_UV;
//    }
//    vec4 diffuseColor = texture(u_RenderedTexture, uv);
//    out_Col = diffuseColor;
//}

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


uniform float vx_offset = 0.5; //how much effect is on screen
 float hatch_y_offset = 5.0; // 5.0
 float lum_threshold_1 = 1.5; // 1.0
 float lum_threshold_2 = 0.7; // 0.7
 float lum_threshold_3 = 0.5; // 0.5
 float lum_threshold_4 = 0.3; // 0.3
void main()
{

  vec3 tc = vec3(0.9, 0.9, 0.0);
  if (fs_UV.x < (vx_offset-0.005))
  {
    float lum = length(texture(u_RenderedTexture, fs_UV).rgb);
    tc = vec3(1.0, 1.0, 1.0);

    if (lum < lum_threshold_1)
    {
      if (mod(gl_FragCoord.x + gl_FragCoord.y, 10.0) == 0.0)
        tc = vec3(0.0, 0.0, 0.0);
    }

    if (lum < lum_threshold_2)
    {
      if (mod(gl_FragCoord.x - gl_FragCoord.y, 10.0) == 0.0)
        tc = vec3(0.0, 0.0, 0.0);
    }

    if (lum < lum_threshold_3)
    {
      if (mod(gl_FragCoord.x + gl_FragCoord.y - hatch_y_offset, 10.0) == 0.0)
        tc = vec3(0.0, 0.0, 0.0);
    }

    if (lum < lum_threshold_4)
    {
      if (mod(gl_FragCoord.x - gl_FragCoord.y - hatch_y_offset, 10.0) == 0.0)
        tc = vec3(0.0, 0.0, 0.0);
    }
  }
  else if (fs_UV.x>=(vx_offset+0.001))
  {
    tc = texture(u_RenderedTexture, fs_UV).rgb;
  }

  out_Col = vec4(tc, 1.0);
}

