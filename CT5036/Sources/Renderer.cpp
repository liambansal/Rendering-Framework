#include "Renderer.h" // File's header.
#include "DebugCamera.h"
#include <iostream>
#include "OBJLoader.h"
#include "ShaderUtilities.h"
#include "Utilities.h"

// Constructor.
Renderer::Renderer() : m_uiProgram(0),
	m_uiLineVBO(0),
	m_uiOBJProgram(0),
	m_uiOBJModelBuffer(),
	m_pDebugCamera(nullptr),
	m_pOBJModel(nullptr),
	m_pLines(nullptr)
{}

// Destructor.
Renderer::~Renderer()
{}

const unsigned int Renderer::GetWindowWidth() const
{
	return m_uiWindowWidth;
}

const unsigned int Renderer::GetWindowHeight() const
{
	return m_uiWindowHeight;
}

const GLuint Renderer::GetProgram() const
{
	return m_uiProgram;
}

bool Renderer::OnCreate()
{
	m_pOBJModel = new OBJModel();

	if (m_pOBJModel->Load("Resources/obj_models/D0208009.obj"))
	{
		// Setup shaders for OBJ model rendering.
		// Create OBJ shader program.
		unsigned int objVertexShader = ShaderUtilities::LoadShader("Resources/Shaders/obj_vertex.glsl", GL_VERTEX_SHADER);
		unsigned int objFragmentShader = ShaderUtilities::LoadShader("Resources/Shaders/obj_fragment.glsl", GL_FRAGMENT_SHADER);
		m_uiOBJProgram = ShaderUtilities::CreateProgram(objVertexShader, objFragmentShader);
		// Set up vertex and index buffer for OBJ rendering.
		glGenBuffers(2, m_uiOBJModelBuffer);
		// Set up vertex buffer data.
		glBindBuffer(GL_ARRAY_BUFFER, m_uiOBJModelBuffer[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else
	{
		std::cout << "Failed to Load Model.\n";
		return false;
	}

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
	m_pDebugCamera = new DebugCamera(this);
	return true;
}

void Renderer::Update(float a_deltaTime)
{
	const float cameraSpeed = 2.0f;
	m_pDebugCamera->FreeMovement(a_deltaTime, cameraSpeed);
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
	m_pDebugCamera->SendShaderData();
	m_pDebugCamera->UpdateProjectionView();
	// Draw a lined grid.
	glBindBuffer(GL_ARRAY_BUFFER, m_uiLineVBO);
	glBufferData(GL_ARRAY_BUFFER, 42 * sizeof(Line), m_pLines, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// Specify where our vertex array is, how many components each vertex has, 
	// the data type of each component and whether the data is normalised or 
	// not.
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);
	glDrawArrays(GL_LINES, 0, 42 * 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glUseProgram(m_uiOBJProgram);
	m_pDebugCamera->UpdateProjectionView();

	for (unsigned int i = 0; i < m_pOBJModel->GetMeshCount(); ++i)
	{
		OBJMesh* pMesh = m_pOBJModel->GetMeshByIndex(i);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiOBJModelBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER,
			pMesh->GetVertices()->size() * sizeof(OBJVertex),
			pMesh->GetVertices()->data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiOBJModelBuffer[1]);
		// Position.
		glEnableVertexAttribArray(0);
		// Normal.
		glEnableVertexAttribArray(1);
		// UV coordinate.
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + OBJVertex::PositionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), ((char*)0) + OBJVertex::NormalOffset);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), ((char*)0) + OBJVertex::UVCoordinateOffset);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			pMesh->GetIndices()->size() * sizeof(unsigned int),
			pMesh->GetIndices()->data(),
			GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, (GLsizei)pMesh->GetIndices()->size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glUseProgram(0);
}

void Renderer::Destroy()
{
	delete m_pOBJModel;
	delete[] m_pLines;
	glDeleteBuffers(1, &m_uiLineVBO);
	ShaderUtilities::DeleteProgram(m_uiProgram);
}