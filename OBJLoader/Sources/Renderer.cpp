#include "Renderer.h" // File's header.
#include <iostream>
#include "Utilities.cpp"

// Constructor.
Renderer::Renderer() : m_pWindow(nullptr)
{}

// Destructor.
Renderer::~Renderer()
{}

bool Renderer::CreateRenderWindow()
{
	unsigned int majorVersion = 4;
	unsigned int minorVersion = 6;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!glfwInit())
	{
		return false;
	}

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

	if (!gladLoadGL())
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}

	majorVersion = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_VERSION_MAJOR);
	minorVersion = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_VERSION_MINOR);
	unsigned int revision = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_REVISION);
	// Output window attributes to inform user of OpenGL version.
	std::cout << "OpenGL version: " << majorVersion << "." << minorVersion << "." << revision << std::endl;
	// Sets up input to be captured from parameter window.
	glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
	return true;
}

void Renderer::UpdateWindow()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	const float vertexPositions[] =
	{
		-0.25f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.25f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.0f, 1.0f,
		0.5f, -0.25f, 0.0f, 1.0f,
		0.25f, 0.5f, 0.0f, 1.0f
	};

	const float vertexColours[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f
	};

	// Create shader program.
	GLuint uiProgram = CreateProgram();
	// Show prompt to close render window.
	std::cout << "Press \"Escape\" to close render window.\n";

	// Loop for keeping the render window open.
	do
	{
		// Set render window's background colour.
		float redValue = 0.5f;
		float greenValue = 0.45f;
		float blueValue = 0.6f;
		float alphaValue = 1.f;
		glClearColor(redValue, greenValue, blueValue, alphaValue);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Enable shaders.
		glUseProgram(uiProgram);
		// Enable the vertex array state, since we're sending in an array of 
		// vertices.
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		// Specify where our vertex array is, how many components each vertex 
		// has, the data type of each component and whether the data is 
		// normalised or not.
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vertexPositions);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vertexColours);
		// Draw to the screen.
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 3, 3);
		
		// Updates the buffer used to render images to the screen.
		glfwSwapBuffers(m_pWindow);
		// Checks triggered events e.g. keyboard input.
		glfwPollEvents();
	} while (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		!glfwWindowShouldClose(m_pWindow));

	glfwTerminate();
}