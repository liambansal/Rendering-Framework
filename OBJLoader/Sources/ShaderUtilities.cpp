#include "ShaderUtilities.h"
#include <glad/glad.h>
#include <iostream>

// Static instance of shader uilities.
ShaderUtilities* ShaderUtilities::m_instance = nullptr;

ShaderUtilities::ShaderUtilities()
{}

ShaderUtilities::~ShaderUtilities()
{
	int unloadCount = 0;
	// Delete any shaders that have not been unloaded.
	for (auto iterator = m_shaders.begin();
		iterator != m_shaders.end();
		++iterator, ++unloadCount)
	{
		glDeleteShader(*iterator);
	}

	std::cout << "Unloading shaders: " + unloadCount;
	unloadCount = 0;

	// Delete any shaders that have not been unloaded.
	for (auto iterator = m_programs.begin();
		iterator != m_programs.end();
		++iterator, ++unloadCount)
	{
		glDeleteProgram(*iterator);
	}

	std::cout << "Unloading programs: " + unloadCount;
}

ShaderUtilities* ShaderUtilities::CreateInstance()
{
	if (!m_instance)
	{
		m_instance = new ShaderUtilities();
	}
	else
	{
		// Print to console that attempt to create multiple instance of shader 
		// utilities.
		std::cout << "Warning: Attempted to create multiple instances of ShaderUtilities.\n";
	}

	return m_instance;
}

ShaderUtilities* ShaderUtilities::GetInstance()
{
	if (!m_instance)
	{
		return ShaderUtilities::CreateInstance();
	}

	return m_instance;
}

void ShaderUtilities::DestroyInstance()
{
	if (!m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
	else
	{
		// Print to console thatattempt to destroy null instance of shader 
		// utilities.
		std::cout << "Warning: Atempt to destroy null instance of ShaderUtilities.\n";
	}
}

unsigned int ShaderUtilities::LoadShader(const char* a_filename, unsigned int a_type)
{
	return ShaderUtilities::GetInstance()->LoadShaderInternal(a_filename, a_type);
}

unsigned int ShaderUtilities::LoadShaderInternal(const char* a_filename, unsigned int a_type)
{
	return 0;
}

void ShaderUtilities::DeleteShader(unsigned int a_shaderID)
{
	ShaderUtilities::GetInstance()->DeleteShaderInternal(a_shaderID);
}

void ShaderUtilities::DeleteShaderInternal(unsigned int a_shaderID)
{
	// Loop through the shaders vector.
	for (auto iterator = m_shaders.begin();
		iterator != m_shaders.end();
		++iterator)
	{
		if (*iterator == a_shaderID)
		{
			glDeleteShader(*iterator);
			m_shaders.erase(iterator);
			break;
		}
	}
}

unsigned int ShaderUtilities::CreateProgram(const int& a_vertexShader, const int& a_fragmentShader)
{
	return ShaderUtilities::GetInstance()->CreateProgramInternal(a_vertexShader, a_fragmentShader);
}

unsigned int ShaderUtilities::CreateProgramInternal(const int& a_vertexShader, const int& a_fragmentShader)
{
	// Boolean value to test or shader program linkage success.
	int success = GL_FALSE;

	// Create a shader program and attach the shaders to it.
	unsigned int handle = glCreateProgram();
	glAttachShader(handle, a_vertexShader);
	glAttachShader(handle, a_fragmentShader);
	// Link the shader together into one shader program.
	glLinkProgram(handle);
	// Test to see if the program was successfully created.
	glGetProgramiv(handle, GL_LINK_STATUS, &success);

	if (GL_FALSE == success)
	{
		// Integer value to tell us the length of the error log.
		int infoLogLength = 0;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
		// Allocate enough space in a buffer for the error message.
		char* infoLog = new char[infoLogLength];
		// Fill the buffer with data.
		glGetProgramInfoLog(handle, infoLogLength, 0, infoLog);
		// Print log message to console.
		std::cout << "Shader Linker Error.\n";
		std::cout << infoLog << std::endl;
		// Delete the char buffer now that we have displayed it.
		delete[] infoLog;
		// Return 0, programID is a null program.
		return 0;
	}

	// Add the program to the shader program vector.
	m_programs.push_back(handle);
	// Return the program ID.
	return handle;
}

void ShaderUtilities::DeleteProgram(unsigned int a_program)
{
	ShaderUtilities::GetInstance()->DeleteProgramInternal(a_program);
}

void ShaderUtilities::DeleteProgramInternal(unsigned int a_program)
{
	// Loop through the programs vector.
	for (auto iterator = m_programs.begin();
		iterator != m_programs.end();
		++iterator)
	{
		glDeleteProgram(*iterator);
		m_programs.erase(iterator);
		break;
	}
}