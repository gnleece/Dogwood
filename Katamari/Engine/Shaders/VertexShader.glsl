#version 150

in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 texcoord;

out vec3 Position_worldspace;
out vec3 LightPosition_worldspace;

out vec3 Normal_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 EyeDirection_cameraspace;

out vec3 Color;
out vec2 Texcoord;

//TODO do MVP multiplcation on client side
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 lightpos;
//TODO light colour, power

void main()
{
    gl_Position = proj*view*model*vec4(position, 1.0);

    Color = color;
    Texcoord = texcoord;
    LightPosition_worldspace = lightpos;

    // Position of vertex in worldspace
    Position_worldspace = (model * vec4(position,1)).xyz;

    // Vector from vertex to camera, in camera space
    vec3 Position_cameraspace = (view * model * vec4(position,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - Position_cameraspace;

    // Vector from vertex to light, in camera space
    vec3 LightPosition_cameraspace = (view * vec4(lightpos,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

    // Normal of vertex in camera space
    Normal_cameraspace = ( view * model * vec4(normal,0)).xyz; // TODO Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}