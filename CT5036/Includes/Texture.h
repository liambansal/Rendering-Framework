//////////////////////////////
/// File: Texture.h.
/// Author: Liam Bansal.
/// Date Created: 14/11/2020.
//////////////////////////////

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

// Stores texture data.
// A texture is a data buffer that contains values which relate to pixel colours.
class Texture
{
public:
	Texture();
	~Texture();

	// Function to load a texture from file.
	bool Load(std::string a_filename);
	void Unload();
	const std::string& GetFileName() const;
	unsigned int GetTextureID() const;
	void GetDimensions(unsigned int& a_width, unsigned int& a_height) const;

private:
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
	unsigned int m_uiTextureID;
	std::string m_filename;
};

inline void Texture::GetDimensions(unsigned int& a_width, unsigned int& a_height) const
{
	a_width = m_uiWidth;
	a_height = m_uiHeight;
}

#endif // !TEXTURE_H.
