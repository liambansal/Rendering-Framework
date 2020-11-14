//////////////////////////////
/// File: Texture.cpp.
/// Author: Liam Bansal.
/// Date Created: 14/11/2020.
//////////////////////////////

#include "Texture.h" // File's header.
#include <GLAD/glad.h>
#include <iostream>
#include <stb_image.h>

Texture::Texture() : m_uiWidth(0),
	m_uiHeight(0),
	m_uiTextureID(0),
	m_filename()
{}

Texture::~Texture()
{
	Unload();
}

// TODO: use a texture manager for this.
bool Texture::Load(std::string a_filename)
{
	int width = 0;
	int height = 0;
	int channels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(a_filename.c_str(),
		&width,
		&height,
		&channels,
		4);

	if (imageData != nullptr)
	{
		m_uiWidth = width;
		m_uiHeight = height;
		m_filename = a_filename;
		glGenTextures(1, &m_uiTextureID);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(imageData);
		std::cout << "Successfully loaded image file: " << a_filename << std::endl;
		return true;
	}

	std::cout << "Failed to open image file: " << a_filename << std::endl;
	return false;
}

void Texture::Unload()
{
	glDeleteTextures(1, &m_uiTextureID);
}

const std::string& Texture::GetFileName() const
{
	return m_filename;
}

unsigned int Texture::GetTextureID() const
{
	return m_uiTextureID;
}