#version 330 core
// in
layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;

// uniform values
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

// out
out vec4 vertPosition;

void main()
{
	gl_Position = projMat * viewMat * modelMat * vec4(vertex, 1.0f);
	vertPosition = gl_Position;
}