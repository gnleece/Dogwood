#version 150

in vec2 UV;

out vec3 outColour;

uniform sampler2D myTextureSampler;

void main()
{
    outColour = texture(myTextureSampler, UV).rgb;
}