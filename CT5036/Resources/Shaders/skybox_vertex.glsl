//////////////////////////////
/// File: skybox_vertex.glsl.
/// Author: Liam Bansal.
/// Date Created: 05/01/2021.
//////////////////////////////

#version 460
layout (location = 0) in vec3 aPosition;

out vec3 textureCoordinates;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	textureCoordinates = aPosition;
    gl_Position = projection * view * vec4(aPosition, 1.0);
}