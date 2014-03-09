#version 150

in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 texcoord;

out vec3 Normal_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;
out float Distance;
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
 
	// Position of the vertex, in worldspace : model * position
	Position_worldspace = (model * vec4(position,1)).xyz;
	Distance = distance(Position_worldspace, lightpos);
 
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( view * model * vec4(position,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
 
	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( view * vec4(lightpos,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
 
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( view * model * vec4(normal,0)).xyz; // TODO Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}