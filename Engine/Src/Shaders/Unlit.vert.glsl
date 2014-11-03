#version 150

in vec3 position;
in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vertColor;

void main()
{
    gl_Position = proj*view*model*vec4(position, 1.0);
    vertColor = color;
}