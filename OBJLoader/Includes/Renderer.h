#ifndef RENDERER_H
#define RENDERER_H

// Includes.
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

class Renderer
{
public:
	// Constructor.
	Renderer();
	// Destructor.
	~Renderer();

	bool CreateRenderWindow();
	void UpdateWindow();

protected:
	const int m_windowWidth;
	const int m_windowHeight;
	GLuint m_uiProgram;

private:
	GLFWwindow* m_pWindow;
};

#endif // RENDERER_H.