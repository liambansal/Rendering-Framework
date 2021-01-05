//////////////////////////////
/// File: Cubemap.h.
/// Author: Liam Bansal.
/// Date Created: 05/01/2021.
//////////////////////////////

#ifndef CUBEMAP
#define CUBEMAP

#include <string>
#include <vector>

class Cubemap
{
public:
	Cubemap();
	~Cubemap();

	unsigned int Load(std::vector<std::string> textures_faces);
};

#endif // !CUBEMAP.
