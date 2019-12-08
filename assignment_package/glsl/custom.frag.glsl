#version 150

in vec2 fs_UV;
uniform int u_Time;
out vec4 out_Col;
vec3 color;

uniform sampler2D u_RenderedTexture;

void main()
{
    vec4 diffuseColor = texture(u_RenderedTexture, fs_UV);
    color = vec3(diffuseColor.rgb);

    float t = u_Time/20.0;
    vec2 center = vec2(
        sin(t * 1.25 + 75.0 + fs_UV.y * 0.5) + sin(t * 2.75 - 18.0 - fs_UV.x * 0.25),
        sin(t * 1.75 - 125.0 + fs_UV.x * 0.25) + sin(t * 2.25 + 4.0 - fs_UV.y * 0.5)
    ) * 0.25 + 0.5;

    vec2 uv2 = (fs_UV - center) * 1.0 + center;

    float vignette = 1.0 - distance(fs_UV, vec2(0.5));
    color = mix(color, color * vignette, sin((t + 80.023) * 2.0) * 0.75);
    out_Col = vec4(color,1);
}

