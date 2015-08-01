//-VERT-SHADER-

#version 150

in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj*view*model*vec4(position, 1.0);
}

//-FRAG-SHADER-

#version 150

uniform vec3 color;

out vec4 outColor;

void main() 
{
    outColor =	vec4(color,1.0);
}