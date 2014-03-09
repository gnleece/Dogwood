#version 150

in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 Position_worldspace;
in float Distance;
in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;

void main() 
{
    // Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );

	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );

	float cosTheta = clamp( dot( n, l ), 0,1 );
 
	//TODO light color, light power
	outColor = 15*texture(tex, Texcoord)*vec4(Color, 1.0)*cosTheta / (Distance*Distance);
}