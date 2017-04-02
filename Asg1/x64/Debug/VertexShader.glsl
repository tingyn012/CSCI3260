#version 400

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 modelTransformMatrix;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColor;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	vec4 v =  vec4(vertexPosition_modelspace,1);
	vec4 newPosition = modelTransformMatrix * v;
	vec4 projectedPosition = MVP * newPosition;
	gl_Position = projectedPosition;

    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    fragmentColor = vertexColor;

}
