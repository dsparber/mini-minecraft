#include "postprocessshader.h"
#include <QDateTime>

PostProcessShader::PostProcessShader(OpenGLContext *context)
    : ShaderProgram(context),
      unifDimensions(-1),
      unifSampler2D(-1),
      unifTime(-1)
{}

PostProcessShader::~PostProcessShader()
{}

void PostProcessShader::create(const char *vertfile, const char *fragfile)
{
    ShaderProgram::create(vertfile, fragfile);
    unifTime = context->glGetUniformLocation(prog, "u_Time");
    unifSampler2D = context->glGetUniformLocation(prog, "u_RenderedTexture");
}

void PostProcessShader::setDimensions(glm::ivec2 dims)
{
    useMe();

    if(unifDimensions != -1)
    {
        context->glUniform2i(unifDimensions, dims.x, dims.y);
    }
}


void PostProcessShader::setTime(int t)
{
    useMe();

    if(unifTime != -1)
    {
        context->glUniform1i(unifTime, t);
    }
}
