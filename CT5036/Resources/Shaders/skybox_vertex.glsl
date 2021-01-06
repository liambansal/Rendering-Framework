//////////////////////////////
/// File: skybox_vertex.glsl.
/// Author: Liam Bansal.
/// Date Created: 05/01/2021.
//////////////////////////////

#version 460
layout (location = 0) in vec3 aPos;

out vec3 textureCoordinates;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	textureCoordinates = aPos;
    vec4 position = projection * view * vec4(aPos, 1.0);
    gl_Position = position.xyww;
}