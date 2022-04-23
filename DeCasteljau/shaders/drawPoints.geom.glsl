#version 400 core
#pragma optimize(on)
layout(points) in;

layout(triangle_strip, max_vertices = 4) out;

uniform vec2 u_radius;

out vec2 corners;

void main()
{
    gl_Position = gl_in[0].gl_Position + vec4(-u_radius.x, -u_radius.y, 0.0f, 0.0f);
    corners = vec2(-1, -1);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-u_radius.x, u_radius.y, 0.0f, 0.0f);
    corners = vec2(-1, 1);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(u_radius.x, -u_radius.y, 0.0f, 0.0f);
    corners = vec2(1, -1);
    EmitVertex();
        
    gl_Position = gl_in[0].gl_Position + vec4(u_radius.x, u_radius.y, 0.0f, 0.0f);
    corners = vec2(1, 1);
    
    EmitVertex();

    EndPrimitive();
}