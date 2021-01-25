//////////////////////////////
// File: vertex.glsl.
// Author: Liam Bansal.
// Date Created: 29/09/2020.
//////////////////////////////

#version 460

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;

smooth out vec4 vertexColour;

uniform mat4 projectionViewMatrix;

void main()
{
	vertexColour = colour;
	gl_Position = projectionViewMatrix * position;
}