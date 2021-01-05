//////////////////////////////
/// File: skybox_fragment.glsl.
/// Author: Liam Bansal.
/// Date Created: 05/01/2021.
//////////////////////////////


#version 460
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, TexCoords);
}