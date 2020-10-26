#version 460
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 uvCoord;
smooth out vec4 vertexColour;
smooth out vec4 vertexNormal;
uniform mat4 projectionViewMatrix;

void main()
{
	// Sets the colour to grey.
	vertexColour = vec4(0.5f, 0.5f, 0.5f, 1.f);
	vertexNormal = normal;
	gl_Position = projectionViewMatrix * position;
}