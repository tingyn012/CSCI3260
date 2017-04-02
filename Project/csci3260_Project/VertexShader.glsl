#version 400

in layout(location=0) vec3 position;
in layout(location=1) vec2 vertexUV;
in layout(location=2) vec3 normal;

uniform mat4 fulltransformMatrix;
uniform mat4 modelTransformMatrix;
uniform mat4 View;
uniform vec3 lightPosition;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;


void main()
{
	UV = vertexUV;
	vec4 v = vec4(position, 1.0);
	gl_Position = fulltransformMatrix * modelTransformMatrix * v;
	Normal_cameraspace =  (modelTransformMatrix * vec4(normal , 0)).xyz;
	Position_worldspace  = (modelTransformMatrix * v).xyz;
}