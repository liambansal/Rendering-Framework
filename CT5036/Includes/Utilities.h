//////////////////////////////
// File: Utilities.h.
// Author: Liam Bansal.
// Date Created: 28/09/2020.
//////////////////////////////

#ifndef UTILITIES_H
#define UTILITIES_H

#ifdef WIN64
#include "GLAD/glad.h"
#endif // WIN64.

#ifdef NX64
#include <nn/nn_Log.h>
#include <nn/gll.h>
#endif // NX64.

/// <summary>
/// A utility class with static helper methods for shader and timers.
/// </summary>
class Utilities
{
public:
	static GLuint CreateShader(const char* a_strShaderFile, unsigned int a_eShaderType);
	static GLuint CreateProgram();

	// Utilities for timing.
	static void ResetTimer();
	static float TickTimer();
	static float GetDeltaTime();
	static float GetTotalTime();
	// Helper function for loading shader code into memory.
	static char* FileToBuffer(const char* a_sPath);
};

#endif // UTILITIES_H.