//////////////////////////////
/// File: TextureManager.cpp.
/// Author: Liam Bansal.
/// Date Created: 14/11/2020.
//////////////////////////////

#include "TextureManager.h" // File's header.
#include "Texture.h"

// Set up static pointer for singleton object.
TextureManager* TextureManager::m_instance = nullptr;

TextureManager::TextureManager() : m_pTextureMap()
{}

TextureManager::~TextureManager()
{
	m_pTextureMap.clear();
}

TextureManager* TextureManager::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new TextureManager();
	}

	return m_instance;
}

TextureManager* TextureManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		return TextureManager::CreateInstance();
	}

	return m_instance;
}

void TextureManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

// Uses an std map as a texture directory and reference counting.
unsigned int TextureManager::LoadTexture(const char* a_pFilename)
{
	if (a_pFilename != nullptr)
	{
		auto dictionaryIterator = m_pTextureMap.find(a_pFilename);

		if (dictionaryIterator != m_pTextureMap.end())
		{
			// Texture is already in map, increment reference and return 
			// texture ID.
			TextureReference& textureReference = (TextureReference&)(dictionaryIterator->second);
			++textureReference.referenceCount;
			return textureReference.pTexture->GetTextureID();
		}
		// Texture is not in dictionary. Load it in from file.
		else
		{
			Texture* pTexture = new Texture();

			if (pTexture->Load(a_pFilename))
			{
				// Successful load.
				unsigned int referenceCount = 1;
				TextureReference textureReference = { pTexture, referenceCount };
				m_pTextureMap[a_pFilename] = textureReference;
				return pTexture->GetTextureID();
			}
			else
			{
				delete pTexture;
				return 0;
			}
		}
	}

	return 0;
}

unsigned int TextureManager::GetTexture(const char* a_pFilename)
{
	auto dictionaryIterator = m_pTextureMap.find(a_pFilename);

	if (dictionaryIterator != m_pTextureMap.end())
	{
		TextureReference& textureReference =
			(TextureReference&)(dictionaryIterator->second);
		++textureReference.referenceCount;
		return textureReference.pTexture->GetTextureID();
	}

	return 0;
}

bool TextureManager::TextureExists(const char* a_pTextureName)
{
	auto dictionaryIterator = m_pTextureMap.find(a_pTextureName);
	return (dictionaryIterator != m_pTextureMap.end());
}

void TextureManager::ReleaseTexture(unsigned int a_texture)
{
	for (auto dictionaryIterator = m_pTextureMap.begin();
		dictionaryIterator != m_pTextureMap.end();
		++dictionaryIterator)
	{
		TextureReference& textureReference =
			(TextureReference&)(dictionaryIterator->second);

		if (a_texture == textureReference.pTexture->GetTextureID())
		{
			if (--textureReference.referenceCount == 0)
			{
				delete textureReference.pTexture;
				textureReference.pTexture = nullptr;
				m_pTextureMap.erase(dictionaryIterator);
				break;
			}
		}
	}
}
