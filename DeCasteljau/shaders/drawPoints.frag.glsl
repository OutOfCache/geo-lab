#version 400 core
#pragma optimize(on)

out vec4 fragColor;
in vec2 corners;
uniform vec3 u_color;
void main()
{
    if(length(corners) < 1.0)
    {
        fragColor = vec4(u_color.xyz, 1.0f);
    }        
    else
    {
        discard;
    }        
}
