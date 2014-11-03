#version 150

uniform vec3 matColorDiffuse;

out vec4 outColor;

void main() 
{
    outColor =	vec4(matColorDiffuse,1.0);
}