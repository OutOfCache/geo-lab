#version 400 core
#pragma optimize(on)

in vec2 inVertex;

uniform mat3 u_transformationMatrix;

void main()
{	    
    gl_Position	= vec4(inVertex.xy, 0.0, 1.0);
}
