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
	Renderer* pRenderer = new Renderer();
	const unsigned int windowWidth = 1920;
	const unsigned int windowHeight = 1080;
	pRenderer->Run("My Application", windowWidth, windowHeight, false);
	delete pRenderer;
#ifdef WIN64
	return 0;
#endif // WIN64.
}