#ifndef UTILITIES_H
#define UTILITIES_H

// A utility class with static helper methods.
class Utilities
{
public:
	// Utilities for timing.
	static void ResetTimer();
	static float TickTimer();
	// Get() updates timers and returns time since last Get call.
	static float GetDeltaTime();
	static float GetTotalTime();

	// Helper function for loading shader code into memory.
	static char* fileToBuffer(const char* a_szPath);
};

#endif // UTILITIES_H.