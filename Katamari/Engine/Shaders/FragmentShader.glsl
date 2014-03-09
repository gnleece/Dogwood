#version 150

in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 Position_worldspace;
in vec3 EyeDirection_cameraspace;
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
 
	vec4 MaterialDiffuseColor = texture(tex, Texcoord)*vec4(Color, 1.0);
	vec4 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec4 MaterialSpecularColor = vec4(1.0,1.0,1.0,1.0);//MaterialDiffuseColor;

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
 
	//TODO light color, light power, ambient as parameter
	outColor = MaterialAmbientColor + 15*MaterialDiffuseColor*cosTheta / (Distance*Distance) + 150 * MaterialSpecularColor * pow(cosAlpha,5) / (Distance*Distance);
}