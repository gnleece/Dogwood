#version 150

in vec3 Position_worldspace;
in vec3 LightPosition_worldspace;

in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;
uniform vec3 lightColor;
uniform float lightPower;
uniform vec3 materialColor;

void main() 
{
    // material colors
    vec4 MaterialDiffuseColor	= texture(tex, Texcoord)*vec4(materialColor, 1.0);
    vec4 MaterialAmbientColor	= vec4(0.1,0.1,0.1,1.0) * MaterialDiffuseColor;
    vec4 MaterialSpecularColor	= vec4(1.0,1.0,1.0,1.0);

    // cosine of angle between normal and light direction, for diffuse lighting:
    vec3 n = normalize(Normal_cameraspace);
    vec3 l = normalize(LightDirection_cameraspace);
    float cosTheta = clamp(dot(n, l), 0,1);
 
    // cosine of angle between the eye vector and the reflect vector, for specular lighting:
    vec3 E = normalize(EyeDirection_cameraspace);
    vec3 R = reflect(-l,n);
    float cosAlpha = clamp(dot(E,R), 0,1);

    // distance to the light
    float distanceSqrd = pow(distance(Position_worldspace, LightPosition_worldspace),2);

    // combine all lighting types for final color
    //TODO light color, light power, ambient as parameter
    outColor =	MaterialAmbientColor + 
                lightPower * MaterialDiffuseColor * vec4(lightColor,1.0) * cosTheta / distanceSqrd + 
                lightPower/4 * MaterialSpecularColor * vec4(lightColor,1.0) * pow(cosAlpha,5) / distanceSqrd;
}