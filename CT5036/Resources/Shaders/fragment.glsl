//////////////////////////////
// File: fragment.glsl.
// Author: Liam Bansal.
// Date Created: 28/09/2020.
//////////////////////////////

#version 460

smooth in vec4 vertexColour;

out vec4 outputColour;

void main()
{
	outputColour = vertexColour;
}