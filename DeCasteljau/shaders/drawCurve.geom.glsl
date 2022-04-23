

#version 400 core
#pragma optimize(on)
layout(lines_adjacency) in;

layout(triangle_strip, max_vertices = 6) out;

uniform float u_halfLineWidth;
uniform mat3 u_transformationMatrix;

vec4 doIt(vec4 a)
{
    return vec4(a.x, a.y, 0.0, 1.0);
}

void main()
{


    vec2 lineDirectionVector12 = normalize(gl_in[2].gl_Position.xy - gl_in[1].gl_Position.xy);
    vec2 lineNormal12 = vec2(-lineDirectionVector12.y, lineDirectionVector12.x);

    vec2 lineDirectionVector23 = normalize(gl_in[3].gl_Position.xy - gl_in[2].gl_Position.xy);
    vec2 lineNormal23 = vec2(-lineDirectionVector23.y, lineDirectionVector23.x);

   

    gl_Position = doIt(vec4(u_transformationMatrix * (vec3(gl_in[1].gl_Position.xy, 1.0) + u_halfLineWidth * vec3(lineNormal12, 0.0f)), 1.0));
    EmitVertex();

    gl_Position = doIt(vec4(u_transformationMatrix * (vec3(gl_in[1].gl_Position.xy, 1.0) - u_halfLineWidth * vec3(lineNormal12, 0.0f)), 1.0));
    EmitVertex();

    gl_Position = doIt(vec4(u_transformationMatrix * (vec3(gl_in[2].gl_Position.xy, 1.0) + u_halfLineWidth * vec3(lineNormal12, 0.0f)), 1.0));
    EmitVertex();

    gl_Position = doIt(vec4(u_transformationMatrix * (vec3(gl_in[2].gl_Position.xy, 1.0) - u_halfLineWidth * vec3(lineNormal12, 0.0f)), 1.0));
    EmitVertex();

    gl_Position = doIt(vec4(u_transformationMatrix * (vec3(gl_in[2].gl_Position.xy, 1.0) + u_halfLineWidth * vec3(lineNormal23, 0.0f)), 1.0));
    EmitVertex();
 
    gl_Position = doIt(vec4(u_transformationMatrix * (vec3(gl_in[2].gl_Position.xy, 1.0) - u_halfLineWidth * vec3(lineNormal23, 0.0f)), 1.0));
    EmitVertex();


    EndPrimitive();
}