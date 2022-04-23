#version 400 core
#pragma optimize(on)

out vec4 fragColor;
uniform vec3 u_color;
void main()
{
    fragColor = vec4(u_color.xyz, 1.0f);
}
