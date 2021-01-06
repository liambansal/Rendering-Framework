//////////////////////////////
/// File: cubemap_fragment.glsl.
/// Author: Liam Bansal.
/// Date Created: 05/01/2021.
//////////////////////////////

in vec3 textureDirection; // direction vector representing a 3D texture coordinate
uniform samplerCube cubemap; // cubemap texture sampler

void main()
{             
	FragColor = texture(cubemap, textureDirection);
}