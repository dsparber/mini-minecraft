#version 150

in vec2 fs_UV;

out vec4 out_Col;
vec3 color;

uniform sampler2D u_RenderedTexture;

void main()
{
    vec4 diffuseColor = texture(u_RenderedTexture, fs_UV);
    vec3 c = vec3(diffuseColor.rgb);
    color = vec3(0.21 * c.r + 0.72 * c.g + 0.07 * c.b);

    //vignette
    //find the distance at which a fragment lies from the screen's center
    float dist = sqrt(pow(fs_UV.x - 0.5, 2) + pow(fs_UV.y - 0.5, 2));
    //larger distance, darker brightness.
    color = (1 - dist * 1)  * color;
    out_Col = vec4(color,1);
}
