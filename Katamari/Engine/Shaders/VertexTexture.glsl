#version 150

in vec3 position;
in vec2 UV;

out vec2 outUV;

//TODO do MVP multiplcation on client side
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj*view*model*vec4(position, 1.0);

    outUV = UV;
}