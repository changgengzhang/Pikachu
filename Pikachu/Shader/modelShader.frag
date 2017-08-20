#version 330 core

in VSOUT
{
	vec4 position;
	vec2 textCoord;
}fsIn;

out vec4 fragColor;

void main()
{
    fragColor = normalize(fsIn.position) + 0.5;
} 