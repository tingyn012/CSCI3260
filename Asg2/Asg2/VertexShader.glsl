#version 400

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normal;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 normalWorld;
out vec3 vertexPositionWorld;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 modelTransformMatrix;

void main(){

    // Output position of the vertex, in clip space : MVP * position
	vec4 v =  vec4(vertexPosition_modelspace,1);
	vec4 newPosition = modelTransformMatrix * v;
	vec4 projectedPosition = MVP * newPosition;
    gl_Position = projectedPosition;

	vec4 normal_temp = MVP * vec4(normal, 0);
	normalWorld = normal_temp.xyz;

	vertexPositionWorld = newPosition.xyz;

    // UV of the vertex. No special space for this one.
    UV = vertexUV;
}