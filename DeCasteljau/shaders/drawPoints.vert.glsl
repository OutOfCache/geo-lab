#version 400 core
#pragma optimize(on)

in vec2 inVertex;

uniform vec2 u_aspectCorrection;
uniform mat3 u_transformationMatrix;



void main()
{	
    vec3 v = u_transformationMatrix * vec3(inVertex.xy, 1);
    gl_Position	= vec4(v.xy, 0.0, 1.0);   
}
