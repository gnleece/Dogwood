#version 150

in vec3 position;
in vec3 colour;

out vec3 Color;

//TODO do MVP multiplcation on client side
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj*view*model*vec4(position, 1.0);

    Color = colour;
}