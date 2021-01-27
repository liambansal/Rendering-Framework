//////////////////////////////
// File: Utilities.cpp.
// Author: Liam Bansal.
// Date Created: 28/09/2020.
//////////////////////////////

#include "Utilities.h" // File's header.
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifdef WIN64
#include "GLFW/glfw3.h"
#endif // WIN64.

#ifdef NX64
#include <nn/nn_Log.h>
#include <nn/gll.h>
#include <nn/nn_TimeSpan.h>
#include <nn/os.h>
#endif // NX64.

static double s_previousTime = 0;
static float s_totalTime = 0;
static float s_deltaTime = 0;

GLuint Utilities::CreateShader(const char* a_strShaderFile, unsigned int a_eShaderType)
{
	std::string strShaderCode;
	// Open shader file.
	std::ifstream shaderStream(a_strShaderFile);

	// If that worked okay, load file in line by line.
	if (shaderStream.is_open())
	{
		std::string line = "";

		while (std::getline(shaderStream, line))
		{
			strShaderCode += "\n" + line;
		}

		shaderStream.close();
	}

	// Convert to cstring.
	char const* szShaderSourcePointer = strShaderCode.c_str();
	// Create shader ID.
	GLuint uiShader = glCreateShader(a_eShaderType);
	const GLsizei elementCount = 1;
	// Load source code.
	glShaderSource(uiShader, elementCount, &szShaderSourcePointer, NULL);
	// Compile shader.
	glCompileShader(uiShader);
	// Check for compilation errors and output them.
	GLint status;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(uiShader, infoLogLength, NULL, strInfoLog);
		const char* strShaderType = NULL;

		switch (a_eShaderType)
		{
		case GL_VERTEX_SHADER:
			strShaderType = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			strShaderType = "fragment";
			break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n",
			strShaderType,
			strInfoLog);
		delete[] strInfoLog;
	}

	return uiShader;
}

GLuint Utilities::CreateProgram()
{
	std::vector<GLuint> shaderList;
	shaderList.push_back(CreateShader("Resources/Shaders/vertex.glsl",
		GL_VERTEX_SHADER));
	shaderList.push_back(CreateShader("Resources/Shaders/fragment.glsl",
		GL_FRAGMENT_SHADER));
	// Create shader program ID.
	GLuint uiProgram = glCreateProgram();

	// Attach shaders.
	for (auto pShader = shaderList.begin(); pShader != shaderList.end(); ++pShader)
	{
		glAttachShader(uiProgram, *pShader);
	}

	// Link program.
	glLinkProgram(uiProgram);
	// Check for link errors and output them.
	GLint status;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(uiProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(uiProgram, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (auto shader = shaderList.begin(); shader != shaderList.end(); ++shader)
	{
		glDetachShader(uiProgram, *shader);
		glDeleteShader(*shader);
	}

	return uiProgram;
}

void Utilities::ResetTimer()
{
#ifdef WIN64
	s_previousTime = glfwGetTime();
#elif NX64
	nn::os::Tick tick = nn::os::GetSystemTick();
	nn::TimeSpan timeSpan = nn::os::ConvertToTimeSpan(tick);
	s_previousTime = timeSpan.GetMilliSeconds() / 1000.0;
#endif // WIN64 / NX64.
	s_totalTime = 0;
	s_deltaTime = 0;
}

float Utilities::TickTimer()
{
#ifdef WIN64
	double currentTime = glfwGetTime();
#elif NX64
	nn::os::Tick tick = nn::os::GetSystemTick();
	nn::TimeSpan timeSpan = nn::os::ConvertToTimeSpan(tick);
	double currentTime = timeSpan.GetMilliSeconds() / 1000.0;
#endif // WIN64 / NX64.
	s_deltaTime = (float)(currentTime - s_previousTime);
	s_totalTime += s_deltaTime;
	s_previousTime = currentTime;
	return s_deltaTime;
}

float Utilities::GetDeltaTime()
{
	return s_deltaTime;
}

float Utilities::GetTotalTime()
{
	return s_totalTime;
}

char* Utilities::FileToBuffer(const char* a_sPath)
{
	// Get an fstream to read in the file data.
	std::fstream file;
	file.open(a_sPath, std::ios_base::in | std::ios_base::binary);

	// Test to see if the file has opened correctly.
	if (file.is_open())
	{
		// Success. File has been opened, verify contents of file.
		// Attmempt to read the highest number of bytes from the file.
		file.ignore(std::numeric_limits<std::streamsize>::max());
		// gCount will have reached end of file marker, letting us know the 
		// number of bytes.
		std::streamsize fileSize = file.gcount();
		// Clear end of file marker from being read.
		file.clear();
		// Seek back to the beginning of the file.
		file.seekg(0, std::ios_base::beg);

		// If our file has no data close the file and return early.
		if (fileSize == 0)
		{
			file.close();
			return nullptr;
		}

		// Create a char buffer large enough to hold the entire file.
		char* dataBuffer = new char[fileSize + 1];
		// Ensure the contents of the buffer are cleared.
		memset(dataBuffer, 0, fileSize + 1);
		// Fill the buffer with the contents of the file.
		file.read(dataBuffer, fileSize);
		// Close the file.
		file.close();
		return dataBuffer;
	}

	return nullptr;
}