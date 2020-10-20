#ifndef UTILITIES_H
#define UTILITIES_H

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

// A utility class with static helper methods.
class Utilities
{
public:
	static GLuint CreateShader(const char* a_strShaderFile, unsigned int a_eShaderType);
	static GLuint CreateProgram();

	// Utilities for timing.
	static void ResetTimer();
	static float TickTimer();
	// Get() updates timers and returns time since last Get call.
	static float GetDeltaTime();
	static float GetTotalTime();

	// Helper function for loading shader code into memory.
	static char* fileToBuffer(const char* a_sPath);
};

#endif // UTILITIES_H.