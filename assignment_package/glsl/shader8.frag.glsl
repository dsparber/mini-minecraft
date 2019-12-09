#version 150
// shader8.vert.glsl:

in vec4 vs_Pos;
in vec2 vs_UV;

out vec2 fs_UV;
out vec4 fs_Pos;

void main()
{
    fs_UV = vs_UV;
    gl_Position = vs_Pos;
    fs_Pos = vs_Pos;
}
