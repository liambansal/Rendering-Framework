#version 460
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;
smooth out vec4 vertexColour;

void main()
{
	vertexColour = colour;
	gl_Position = position;
}