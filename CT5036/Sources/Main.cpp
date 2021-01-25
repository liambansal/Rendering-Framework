//////////////////////////////
// File: Main.cpp.
// Author: Liam Bansal.
// Date Created: 21/09/2020.
//////////////////////////////

#include "Renderer.h"

#ifdef WIN64
int main()
#elif NX64
extern "C" void nnMain()
#endif // WIN64 / NX64.
{
	Renderer* renderer = new Renderer();
	renderer->Run("My Application", 1920, 1080, false);
	delete renderer;
#ifdef WIN64
	return 0;
#endif // WIN64.
}