#version 400

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform sampler2D myTextureSampler_2;

uniform vec3 ambientLight;
uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;
uniform vec3 Diff_brightness;

uniform int spec_brightness;

void main(){

	// Diffuse
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);


	// Specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	s = pow(s, spec_brightness);
	// vec4 specularLight = vec4(s, s, s, 1);


	vec3 MaterialAmbientColor = 0.5 * texture( myTextureSampler, UV ).rgb + 0.5 * texture( myTextureSampler_2, UV ).rgb;
	vec3 MaterialDiffuseColor = 0.5 * texture( myTextureSampler, UV ).rgb + 0.5 * texture( myTextureSampler_2, UV ).rgb;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

    // Output color = color of the texture at the specified UV
	color = MaterialAmbientColor * ambientLight + MaterialDiffuseColor * clamp(diffuseLight, 0 , 1) * Diff_brightness + MaterialSpecularColor * s;
	// color = MaterialDiffuseColor  * clamp(diffuseLight, 0 , 1) ;


}
