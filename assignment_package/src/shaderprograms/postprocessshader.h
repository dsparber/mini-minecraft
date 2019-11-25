#pragma once

#include "shaderprogram.h"

class PostProcessShader : public ShaderProgram
{
public:
    int unifDimensions; // A handle to the "uniform" ivec2 that stores the width and height of the texture being rendered

    int unifSampler2D; // A handle to the "uniform" sampler2D that will be used to read the texture containing the scene render
    int unifTime; // A handle for the "uniform" float representing time in the shader


public:
    PostProcessShader(OpenGLContext* context);
    virtual ~PostProcessShader();

    // Sets up shader-specific handles
    void setupMemberVars();

    void create(const char *vertfile, const char *fragfile);

    void setDimensions(glm::ivec2 dims);

    void setTime(int t);
};
