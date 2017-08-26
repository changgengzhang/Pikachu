#version 330 core

in VSOUT
{
	vec4 position;
	vec2 textCoord;
}fsIn;

out vec4 fragColor;

uniform sampler2D image;

void main()
{
    fragColor = texture(image, fsIn.textCoord);
} 