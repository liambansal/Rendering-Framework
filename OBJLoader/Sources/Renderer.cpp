#include "Renderer.h" // File's header.
#include <iostream>

// Constructor.
Renderer::Renderer() : m_pWindow(nullptr)
{}

// Destructor.
Renderer::~Renderer()
{}

bool Renderer::CreateRenderWindow()
{
	glfwInit();
	int openGLVersion = 4;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int windowWidth = 1024;
	int windowHeight = 640;
	m_pWindow = glfwCreateWindow(windowWidth,
		windowHeight,
		"OpenGL OBJLoader",
		NULL,
		NULL);

	if (!m_pWindow)
	{
		std::cout << "Failed to create GLFW window./n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_pWindow);
	// Sets up input to be captured from parameter window.
	glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
	return true;
}

void Renderer::UpdateWindow(GLuint* a_pVertexBuffer)
{
	// Loop for keeping the render window open.
	do
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
		unsigned int verteciesToDraw = 4;
		// Draw a quad.
		glDrawArrays(GL_QUADS, 0, verteciesToDraw);
		glDisableVertexAttribArray(0);
		
		// Updates the buffer used to render images to the screen.
		glfwSwapBuffers(m_pWindow);
		// Checks triggered events e.g. keyboard input.
		glfwPollEvents();
	} while (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		!glfwWindowShouldClose(m_pWindow));
}