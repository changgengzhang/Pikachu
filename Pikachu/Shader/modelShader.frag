#version 330 core

in VSOUT
{
	vec4 position;
	vec2 textCoord;
}fsIn;

out vec4 fragColor;

uniform sampler2D image;
uniform bool hasTexture;

void main()
{
	if(hasTexture)
	{
		fragColor = texture(image, fsIn.textCoord);
	}
	else
	{
		fragColor = fsIn.position + 0.5;
	}
} 