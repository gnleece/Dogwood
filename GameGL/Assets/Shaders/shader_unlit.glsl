//-VERT-SHADER-

#version 150

in vec3 position;
in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 vertColor;

void main()
{
    gl_Position = proj*view*model*vec4(position, 1.0);
    vertColor = color;
}

//-FRAG-SHADER-

#version 150

in vec3 vertColor;

out vec4 outColor;

void main() 
{
    outColor =	vec4(vertColor,1.0);
}