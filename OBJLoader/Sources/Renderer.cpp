#include "Renderer.h" // File's header.
#include "DebugCamera.h"
#include <iostream>
#include "Utilities.h"

// Constructor.
Renderer::Renderer() : m_windowWidth(1024),
	m_windowHeight(640),
	m_uiProgram(0),
	m_pWindow(nullptr)
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

	m_pWindow = glfwCreateWindow(m_windowWidth,
		m_windowHeight,
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
	unsigned int revision = glfwGetWindowAttrib(m_pWindow,
		GLFW_CONTEXT_REVISION);
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
	m_uiProgram = Utilities::CreateProgram();

	typedef struct Vertex
	{
		glm::vec4 position;
		glm::vec4 colour;
	} Vertex;

	typedef struct Line
	{
		Vertex v0;
		Vertex v1;
	} Line;

	// Create a grid of lines to be drawn during our update.
	// Create a 10x10 square grid.
	Line* lines = new Line[42];
	const glm::vec4 white(1.f, 1.f, 1.f, 1.f);
	const glm::vec4 black(0.f, 0.f, 0.f, 1.f);

	for (int i = 0, j = 0; i < 21; ++i, j += 2)
	{
		lines[j].v0.position = glm::vec4(-10 + i, 0.f, 10.f, 1.f);
		lines[j].v0.colour = (i == 10) ? white : black;
		lines[j].v1.position = glm::vec4(-10 + i, 0.f, -10.f, 1.f);
		lines[j].v1.colour = (i == 10) ? white : black;

		lines[j + 1].v0.position = glm::vec4(10, 0.f, -10.f + i, 1.f);
		lines[j + 1].v0.colour = (i == 10) ? white : black;
		lines[j + 1].v1.position = glm::vec4(-10, 0.f, -10.f + i, 1.f);
		lines[j + 1].v1.colour = (i == 10) ? white : black;
	}

	// Create a vertex buffer to hold our line data.
	unsigned int lineVBO;
	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	// Fill the vertex buffer with line data.
	glBufferData(GL_ARRAY_BUFFER, 42 * sizeof(Line), lines, GL_STATIC_DRAW);
	// As we have sent the line data to the GPU we no longer require it on the 
	// CPU side memory.
	delete[] lines;
	// Enable the vertex array state since we're semding in an array of 
	// vertices.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// Specify where our vertex array is, how many components each vertex has, 
	// the data type of each componenet and whether the data is normalised.
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Show prompt to close render window.
	std::cout << "Press \"Escape\" to close render window.\n";
	DebugCamera debugCamera;

	// Loop for keeping the render window open.
	do
	{
		debugCamera.UpdateProjectionView();
		const float fixedDeltaTime = 0.016f;
		const float cameraSpeed = 2.0f;
		debugCamera.FreeMovement(fixedDeltaTime, cameraSpeed);

		// Set render window's background colour.
		float redValue = 0.5f;
		float greenValue = 0.45f;
		float blueValue = 0.6f;
		float alphaValue = 1.f;
		glClearColor(redValue, greenValue, blueValue, alphaValue);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Enable shaders.
		glUseProgram(m_uiProgram);
		debugCamera.SendShaderData();
		// Draw a lined grid.
		glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
		glDrawArrays(GL_LINES, 0, 42 * 2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Updates the buffer used to render images to the screen.
		glfwSwapBuffers(m_pWindow);
		// Checks triggered events e.g. keyboard input.
		glfwPollEvents();
	} while (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		!glfwWindowShouldClose(m_pWindow));

	glfwTerminate();
}