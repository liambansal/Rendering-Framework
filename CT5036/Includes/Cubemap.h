//////////////////////////////
/// File: Cubemap.h.
/// Author: Liam Bansal.
/// Date Created: 05/01/2021.
//////////////////////////////

#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include <vector>

/// <summary>
/// Supports the loading of textures in the form of a cubemap.
/// </summary>
class Cubemap
{
public:
	Cubemap();
	~Cubemap();

	unsigned int Load(std::vector<std::string> a_texturesFaces);
};

#endif // !CUBEMAP_H.
