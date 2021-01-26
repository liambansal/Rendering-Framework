//////////////////////////////
/// File: TextureManager.h.
/// Author: Liam Bansal.
/// Date Created: 14/11/2020.
//////////////////////////////

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>
#include <string>

class Texture;

class TextureManager
{
public:
	// This manager class will act as a singleton object for ease of access.
	static TextureManager* CreateInstance();
	static TextureManager* GetInstance();
	static void DestroyInstance();

	unsigned int LoadTexture(const char* a_pFilename);
	unsigned int GetTexture(const char* a_pFilename);
	bool TextureExists(const char* a_pTextureName);
	void ReleaseTexture(unsigned int a_texture);

private:
	// Structure to reference conut a texture.
	typedef struct TextureReference
	{
		Texture* pTexture;
		// Indicates how many pointers are currently pointing to this texture.
		// Only unload at 0 references.
		unsigned int referenceCount;
	} TextureReference;

	TextureManager();
	~TextureManager();

	static TextureManager* m_poInstance;
	std::map<std::string, TextureReference> m_pTextureMap;
};

#endif // !TEXTURE_MANAGER_H.
