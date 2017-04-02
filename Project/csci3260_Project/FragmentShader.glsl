#version 400

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;

out vec3 daColor;

uniform sampler2D myTextureSampler;
uniform vec3 ambientLight;
uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform vec3 DiffuseBrightness;
uniform vec3 specBrightness;

void main()
{
	vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor = ambientLight * MaterialDiffuseColor;

	 vec3 lightVectorWorld = normalize( lightPosition - Position_worldspace );
	 float brightness = dot(lightVectorWorld, normalize(Normal_cameraspace));
	 vec3 DiffuseLight = vec3(brightness, brightness, brightness);
			
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3);
	vec3 R = reflect(-lightVectorWorld,Normal_cameraspace);
	vec3 eyeVectorWorld = normalize(eyePosition - Position_worldspace);
	float s = clamp( dot( R , eyeVectorWorld) , 0, 1);
	s = pow(s, 50);
	vec3 specularLight = vec3(s, s, s);

	 daColor =  MaterialAmbientColor + MaterialDiffuseColor * clamp(DiffuseLight, 0, 1) * DiffuseBrightness+   specularLight *  MaterialSpecularColor * specBrightness;
}
