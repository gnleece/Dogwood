//-VERT-SHADER-

#version 150

in vec3 position;
in vec2 texcoord;

out vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Texcoord = texcoord;
    gl_Position = proj*view*model*vec4(position, 1.0);
}

//-FRAG-SHADER-

#version 150

in vec2 Texcoord;

uniform sampler2D tex;  // [SERIALIZABLE]
uniform vec3 color;     // [SERIALIZABLE]

out vec4 outColor;

void main() 
{
    outColor =	texture(tex, Texcoord)*vec4(color, 1.0);
}