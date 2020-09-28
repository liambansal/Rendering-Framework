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
	void UpdateWindow(GLuint* a_pVertexBuffer);

private:
	GLFWwindow* m_pWindow;
};

#endif // RENDERER_H.