#version 460

smooth in vec4 vertexColour;

out vec4 outputColour;

void main()
{
	outputColour = vertexColour;
}