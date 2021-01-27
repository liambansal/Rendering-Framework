//////////////////////////////
// File: ShaderUtilities.h.
// Author: Liam Bansal.
// Date Created: 12/10/2020.
//////////////////////////////

#ifndef SHADER_UTILITIES_H
#define SHADER_UTILITIES_H

#include <vector>

/// <summary>
/// A utility class for creating shader programs.
/// Class implements a singleton design pattern. 
/// </summary>
class ShaderUtilities
{
public:
	static ShaderUtilities* CreateInstance();
	static ShaderUtilities* GetInstance();
	static void DestroyInstance();
	static unsigned int LoadShader(const char* a_filename,
		unsigned int a_type);
	static void DeleteShader(unsigned int a_shaderID);
	static unsigned int CreateProgram(const int& a_vertexShader,
		const int& a_fragmentShader);
	static void DeleteProgram(unsigned int a_program);

private:
	// Constructor.
	ShaderUtilities();
	// Destructor.
	~ShaderUtilities();

	unsigned int LoadShaderInternal(const char* a_filename,
		unsigned int a_type);
	void DeleteShaderInternal(unsigned int a_shaderID);
	unsigned int CreateProgramInternal(const int& a_vertexShader,
		const int& a_fragmentShader);
	void DeleteProgramInternal(unsigned int a_program);

	static ShaderUtilities* m_poInstance;
	std::vector<unsigned int> m_shaders;
	std::vector<unsigned int> m_programs;
};

#endif // SHADER_UTILITIES_H.