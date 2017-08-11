#version 330 core

in vec4 vertPosition;

out vec4 fragColor;

void main()
{
    fragColor = normalize(vertPosition) + 0.5;
} 