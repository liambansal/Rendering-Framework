#include "Renderer.h"

int main()
{
	Renderer* renderer = new Renderer();
	renderer->Run("My Application", 1920, 1080, false);
	delete renderer;
	return EXIT_SUCCESS;
}