#include "Utilities.h" // File's header.
#include <fstream>
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/ext.hpp"
#include <iostream>
#include <string>
#include <vector>

static double s_previousTime = 0;
static float s_totalTime = 0;
static float s_deltaTime = 0;

void Utilities::ResetTimer()
{
	s_previousTime = glfwGetTime();
	s_totalTime = 0;
	s_deltaTime = 0;
}

float Utilities::TickTimer()
{
	double currentTime = glfwGetTime();
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

static GLuint CreateShader(const char* a_strShaderFile, unsigned int a_eShaderType)
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
	// Load source code.
	glShaderSource(uiShader, 1, &szShaderSourcePointer, NULL);
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

static GLuint CreateProgram()
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