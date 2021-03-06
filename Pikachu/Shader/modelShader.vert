#version 330 core
// in
layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 textCoord;

// uniform values
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;


// out
out VSOUT
{
	vec4 position;
	vec2 textCoord;
}vsOut;


void main()
{
	gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0f);
	vsOut.position = gl_Position;
	vsOut.textCoord = textCoord;
}