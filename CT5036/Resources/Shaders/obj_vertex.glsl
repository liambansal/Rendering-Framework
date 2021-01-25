//////////////////////////////
// File: obj_vertex.glsl.
// Author: Liam Bansal.
// Date Created: 25/10/2020.
//////////////////////////////

#version 460

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 uvCoord;

smooth out vec4 vertexPosition;
smooth out vec4 vertexNormal;
smooth out vec2 vertexUV;

uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;

void main()
{
	vertexUV = uvCoord;
	vertexNormal = normal;
	// World-space position.
	vertexPosition = modelMatrix * position;
	// Screen-space position.
	gl_Position = projectionViewMatrix * modelMatrix * position;
}