#version 150
// ^ Change this to version 130 if you have compatibility issues

// This is a fragment shader. If you've opened this file first, please
// open and read lambert.vert.glsl before reading on.
// Unlike the vertex shader, the fragment shader actually does compute
// the shading of geometry. For every pixel in your program's output
// screen, the fragment shader is run for every bit of geometry that
// particular pixel overlaps. By implicitly interpolating the position
// data passed into the fragment shader by the vertex shader, the fragment shader
// can compute what color to apply to its pixel based on things like vertex
// position, light position, and vertex color.

uniform vec4 u_Color; // The color with which to render this instance of geometry.
uniform sampler2D u_Texture;
uniform int u_Time;
in vec2 fs_UV;
in float fs_cosPow;
in float fs_animatable;

// These are the interpolated values out of the rasterizer, so you can't know
// their specific values without knowing the vertices that contributed to them
in vec4 fs_Nor;
in vec4 fs_LightVec;
in vec4 fs_Col;

in vec4 fs_CameraPos;

out vec4 out_Col; // This is the final output color that you will see on your
                  // screen for the pixel that is currently being processed.

void main()
{  
        vec2 newUV;
        float i = u_Time % 96;
        float offset = 1.f/96.f;
        //set uv to a loop of offset of texture if animatable
        if(fs_animatable > 0){
          newUV = fs_UV - vec2(1/16, 0) + vec2(i*offset/16.f,0);
        } else {
            newUV = fs_UV;
        }

        // Material base color (before shading)
        vec4 H = (fs_LightVec + fs_CameraPos) / 2.f;

        vec4 diffuseColor = texture(u_Texture, newUV) + max(pow(dot(normalize(H), normalize(fs_Nor)), fs_cosPow), 0);

        // Calculate the diffuse term for Lambert shading
        float diffuseTerm = dot(normalize(fs_Nor), normalize(fs_LightVec));
        // Avoid negative lighting values
        diffuseTerm = clamp(diffuseTerm, 0, 1);

        float rise = 0.1;
        float night = -0.1;

        //ambientTerm changes with u_Time
        float freq = 2000.0;
        float dayTime = sin(u_Time/freq*2);
        float wt = abs(sin(u_Time/freq*2));
        float ambientTerm = 0.0;
        float contrast = 1.0;
        if(dayTime < 0){
            //night
            ambientTerm = mix(0, night, wt);
            contrast = mix(0.8, 0.2, wt);
        } else {
            ambientTerm = mix(-0.1, 0.2, wt);
            contrast = mix(1.0, 0.8, wt);
        }


        float lightIntensity = (diffuseTerm  + ambientTerm) * contrast;   //Add a small float value to the color multiplier
                                                            //to simulate ambient lighting. This ensures that faces that are not
                                                            //lit by our point light are not completely black.

        // Compute final shaded color
        out_Col = vec4(diffuseColor.rgb * lightIntensity, diffuseColor.a * fs_Col.a);
        //out_Col = diffuseColor;
}
