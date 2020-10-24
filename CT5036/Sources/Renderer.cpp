#include "Renderer.h" // File's header.
#include "DebugCamera.h"
#include <iostream>
#include "ShaderUtilities.h"
#include "Utilities.h"

// Constructor.
Renderer::Renderer() : m_uiLineVBO(0)
{}

// Destructor.
Renderer::~Renderer()
{}

bool Renderer::OnCreate()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// Create shader program.
	unsigned int vertexShader = ShaderUtilities::LoadShader("Resources/Shaders/vertex.glsl", GL_VERTEX_SHADER);
	unsigned int fragmentShader = ShaderUtilities::LoadShader("Resources/Shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	m_uiProgram = ShaderUtilities::CreateProgram(vertexShader, fragmentShader);
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

	glGenBuffers(1, &m_uiLineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiLineVBO);
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
	m_debugCamera = new DebugCamera();
	return true;
}

void Renderer::Update(float a_deltaTime)
{
	const float cameraSpeed = 2.0f;
	m_debugCamera->FreeMovement(a_deltaTime, cameraSpeed);
}

void Renderer::Draw()
{
	// Set render window's background colour.
	float redValue = 0.5f;
	float greenValue = 0.45f;
	float blueValue = 0.6f;
	float alphaValue = 1.f;
	glClearColor(redValue, greenValue, blueValue, alphaValue);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable shaders.
	glUseProgram(m_uiProgram);
	m_debugCamera->SendShaderData();
	m_debugCamera->UpdateProjectionView();
	// Draw a lined grid.
	glBindBuffer(GL_ARRAY_BUFFER, m_uiLineVBO);
	glDrawArrays(GL_LINES, 0, 42 * 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::Destroy()
{
	glDeleteBuffers(1, &m_uiLineVBO);
	ShaderUtilities::DeleteProgram(m_uiProgram);
}