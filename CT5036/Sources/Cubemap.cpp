//////////////////////////////
/// File: Cubemap.cpp.
/// Author: Liam Bansal.
/// Date Created: 05/01/2021.
//////////////////////////////

#include "Cubemap.h" // File's header.
#include <iostream>
#ifdef WIN64
#include "GLAD/glad.h"
#endif // WIN64.
#include "GLM/glm.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

Cubemap::Cubemap()
{}

Cubemap::~Cubemap()
{}

unsigned int Cubemap::Load(std::vector<std::string> textures_faces)
{
	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width;
	int height;
	int nrChannels;

	for (unsigned int i = 0; i < textures_faces.size(); ++i)
	{
		// TODO FIX: loading with current texture filenames endlessly produces garbage in the console window.
		unsigned char* data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			std::cout << "Successfully loaded cubemap texture: " << data << std::endl;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << textures_faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}