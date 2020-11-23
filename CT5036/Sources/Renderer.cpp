#include "Renderer.h" // File's header.
#include "DebugCamera.h"
#include "GLM/ext.hpp"
#include <iostream>
#include "OBJLoader.h"
#include "ShaderUtilities.h"
#include "TextureManager.h"
#include "Utilities.h"

#ifdef NX64
#include <nn/fs.h>
#include <nn/nn_Abort.h>
#endif

// Constructor.
Renderer::Renderer() : m_uiProgram(0),
m_uiLineVBO(0),
m_uiLinesVAO(0),
m_uiOBJProgram(0),
m_currentProgram(0),
m_uiOBJModelVAO(0),
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

void Renderer::SetProgram(unsigned int program)
{
	glUseProgram(program);
	m_currentProgram = program;
}

const GLuint Renderer::GetProgram() const
{
	return m_currentProgram;
}

const OBJModel* Renderer::GetModel() const
{
	return m_pOBJModel;
}

bool Renderer::OnCreate()
{
	TextureManager::CreateInstance();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

#ifdef WIN64
	// Create shader program.
	unsigned int vertexShader = ShaderUtilities::LoadShader("Resources/Shaders/vertex.glsl", GL_VERTEX_SHADER);
	unsigned int fragmentShader = ShaderUtilities::LoadShader("Resources/Shaders/fragment.glsl", GL_FRAGMENT_SHADER);
#elif NX64
	// The mount name of the resource data.
	const char* mountName = "rom";
	// File path of the test target.
	const char* testTargetPath = "rom:/dummy.bmp";
	// The buffer size when reading.
	const size_t readBufferSize = 4 * 1024;
	// Allocate 128MB of cache memory for file data cache.
	const size_t fileDataCacheSize = 128 * 1024 * 1024;
	void* fileDataCache = std::malloc(fileDataCacheSize);
	NN_ABORT_UNLESS_NOT_NULL(fileDataCache);
	// TODO: initialize.
	// Allocate file system cache memory and mount file system for the 
	// resource data. (This allocated cache memory is used to store file system
	// management information. It has nothing to do with file data cache).
	size_t fileSystemCacheSize;
	NN_ABORT_UNLESS_RESULT_SUCCESS(nn::fs::QueryMountRomCacheSize(&fileSystemCacheSize));
	void* fileSystemCache = std::malloc(fileSystemCacheSize);
	NN_ABORT_UNLESS_NOT_NULL(fileSystemCache);
	NN_ABORT_UNLESS_RESULT_SUCCESS(nn::fs::MountRom(mountName,
		fileSystemCache,
		fileSystemCacheSize));
	// Create shader program.
	unsigned int vertexShader = ShaderUtilities::LoadShader("rom:/shaders/vertex.glsl", GL_VERTEX_SHADER);
	unsigned int fragmentShader = ShaderUtilities::LoadShader("rom:/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
#endif // WIN64 / NX64.

	m_uiProgram = ShaderUtilities::CreateProgram(vertexShader, fragmentShader);
	// Create a grid of lines to be drawn during our update.
	// Create a 10x10 square grid.
	Line* lines = new Line[42];
	const glm::vec4 white(1.f, 1.f, 1.f, 1.f);
	const glm::vec4 black(0.f, 0.f, 0.f, 1.f);

	for (int i = 0, j = 0; i < 21; ++i, j += 2)
	{
		m_pLines[j].v0.position = glm::vec4(-10 + i, 0.f, 10.f, 1.f);
		m_pLines[j].v0.colour = (i == 10) ? white : black;
		m_pLines[j].v1.position = glm::vec4(-10 + i, 0.f, -10.f, 1.f);
		m_pLines[j].v1.colour = (i == 10) ? white : black;

		m_pLines[j + 1].v0.position = glm::vec4(10, 0.f, -10.f + i, 1.f);
		m_pLines[j + 1].v0.colour = (i == 10) ? white : black;
		m_pLines[j + 1].v1.position = glm::vec4(-10, 0.f, -10.f + i, 1.f);
		m_pLines[j + 1].v1.colour = (i == 10) ? white : black;
	}

	glGenBuffers(1, &m_uiLineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiLineVBO);
	// Fill the vertex buffer with line data.
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(Line) * 42, m_pLines, 0);
	//glBufferData(GL_ARRAY_BUFFER, 42 * sizeof(Line), m_pLines, GL_STATIC_DRAW);
	// Generate our vertex array object.
	glGenVertexArrays(1, &m_uiLinesVAO);
	glBindVertexArray(m_uiLinesVAO);
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
	glBindBuffer(GL_ARRAY_BUFFER, m_uiLineVBO);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_pDebugCamera = new DebugCamera(this);
	m_pOBJModel = new OBJModel();

#ifdef WIN64
	if (m_pOBJModel->Load("Resources/obj_models/Model_D0208009/D0208009.obj"))
#elif NX64
	// Slightly changed file path from win64 variant.
	if (m_pOBJModel->Load("rom:/obj_models/Model_D0208009/D0208009.obj"))
#endif // WIN64 / NX64.
	{
		TextureManager* pTextureManager = TextureManager::GetInstance();

		// Load in the model's textures.
		for (int i = 0; i < m_pOBJModel->GetMaterialCount(); ++i)
		{
			OBJMaterial* material = m_pOBJModel->GetMaterialByIndex(i);

			for (int j = 0; j < OBJMaterial::TEXTURE_TYPES::TEXTURE_TYPES_COUNT; ++j)
			{
				if (material->GetTextureFileName(j).size() > 0)
				{
					unsigned int textureID = pTextureManager->LoadTexture(material->GetTextureFileName(j).c_str());
					material->SetTextureID(j, textureID);
				}
			}
		}

		// Setup shaders for OBJ model rendering.
		// Create OBJ shader program.
#ifdef WIN64
		unsigned int objVertexShader = ShaderUtilities::LoadShader("Resources/Shaders/obj_vertex.glsl", GL_VERTEX_SHADER);
		unsigned int objFragmentShader = ShaderUtilities::LoadShader("Resources/Shaders/obj_fragment.glsl", GL_FRAGMENT_SHADER);
#elif NX64
		unsigned int objVertexShader = ShaderUtilities::LoadShader("rom:/Shaders/obj_vertex.glsl", GL_VERTEX_SHADER);
		unsigned int objFragmentShader = ShaderUtilities::LoadShader("rom:/Shaders/obj_fragment.glsl", GL_FRAGMENT_SHADER);
#endif // WIN64 / NX64.

		m_uiOBJProgram = ShaderUtilities::CreateProgram(objVertexShader, objFragmentShader);
		// Set up vertex and index buffer for OBJ rendering.
		glGenBuffers(2, m_uiOBJModelBuffer);
		// Set up vertex buffer data.
		glBindBuffer(GL_ARRAY_BUFFER, m_uiOBJModelBuffer[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiOBJModelBuffer[1]);
		// Position.
		glEnableVertexAttribArray(0);
		// Normal.
		glEnableVertexAttribArray(1);
		// UV Coordinates.
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), ((char*)0) + OBJVertex::PositionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), ((char*)0) + OBJVertex::NormalOffset);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), ((char*)0) + OBJVertex::UVCoordinateOffset);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else
	{
		std::cout << "Failed to Load Model.\n";
		return false;
	}

#ifdef NX64
	// Unmount the file system and free the various memory.
	nn::fs::Unmount(mountName);
	std::free(fileSystemCache);
	std::free(fileDataCache);
#endif // NX64.

	return true;
}

void Renderer::Update(float a_deltaTime)
{
	m_pDebugCamera->FreeMovement(a_deltaTime);
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
	SetProgram(m_uiProgram);
	glBindVertexArray(m_uiLinesVAO);
	m_pDebugCamera->UpdateProjectionView();
	glDrawArrays(GL_LINES, 0, 42 * 2);
	glBindVertexArray(0);
	SetProgram(0);
	SetProgram(m_uiOBJProgram);

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

	SetProgram(0);
	SetProgram(m_uiOBJProgram);
	glBindVertexArray(m_uiOBJModelVAO);
	m_pDebugCamera->UpdateProjectionView();

	for (unsigned int i = 0; i < m_pOBJModel->GetMeshCount(); ++i)
	{
		// Get the model matrix location from the shader program.
		int modelMatrixUnifromLocation =
			glGetUniformLocation(m_currentProgram, "modelMatrix");
		// Send the OBJ model's world matrix data across to the shader program.
		glUniformMatrix4fv(modelMatrixUnifromLocation,
			1,
			false,
			glm::value_ptr(GetModel()->GetWorldMatrix()));
		m_pDebugCamera->UpdateCameraPosition();

		OBJMesh* pMesh = m_pOBJModel->GetMeshByIndex(i);
		OBJMaterial* pMaterial = pMesh->GetMaterial();
		// Send material data to shader.
		int kALocation = glGetUniformLocation(m_uiOBJProgram, "kA");
		int kDLocation = glGetUniformLocation(m_uiOBJProgram, "kD");
		int kSLocation = glGetUniformLocation(m_uiOBJProgram, "kS");

		if (pMaterial)
		{
			// Send the OBJ model's world data across to the shader program.
			glUniform4fv(kALocation, 1, glm::value_ptr(*pMaterial->GetKA()));
			glUniform4fv(kDLocation, 1, glm::value_ptr(*pMaterial->GetKD()));
			glUniform4fv(kSLocation, 1, glm::value_ptr(*pMaterial->GetKS()));

			// Get the location of the diffuse texture.
			int textureUniformLocation = glGetUniformLocation(m_uiOBJProgram, "diffuseTexture");
			// Set diffuse texture to be GL_Texture0.
			glUniform1i(textureUniformLocation, 0);
			// Set the active texture unit to texture0.
			glActiveTexture(GL_TEXTURE0);
			// Bind the texture for diffuse for this material to the texture0.
			glBindTexture(GL_TEXTURE_2D, pMaterial->GetTextureID(OBJMaterial::TEXTURE_TYPES::TEXTURE_TYPES_DIFFUSE));
			textureUniformLocation = glGetUniformLocation(m_uiOBJProgram, "specularTexture");
			glUniform1i(textureUniformLocation, 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, pMaterial->GetTextureID(OBJMaterial::TEXTURE_TYPES::TEXTURE_TYPES_SPECULAR));
			textureUniformLocation = glGetUniformLocation(m_uiOBJProgram, "normalTexture");
			glUniform1i(textureUniformLocation, 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, pMaterial->GetTextureID(OBJMaterial::TEXTURE_TYPES::TEXTURE_TYPES_NORMAL));
		}
		// No material to obtain lighting information from so use defaults.
		else
		{
			// Send the OBJ model's world matrix data across to the shader program.
			glUniform4fv(kALocation, 1, glm::value_ptr(glm::vec4(0.25f, 0.25f, 0.25f, 1.f)));
			glUniform4fv(kDLocation, 1, glm::value_ptr(glm::vec4(1.f, 1.f, 1.f, 1.f)));
			glUniform4fv(kSLocation, 1, glm::value_ptr(glm::vec4(1.f, 1.f, 1.f, 64.f)));
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_uiOBJModelBuffer[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiOBJModelBuffer[1]);
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
		glDrawElements(GL_TRIANGLES,
			pMesh->GetIndices()->size(),
			GL_UNSIGNED_INT,
			0);
	}

	glBindVertexArray(0);
	SetProgram(0);
}

void Renderer::Destroy()
{
	delete m_pOBJModel;
	delete[] m_pLines;
	glDeleteBuffers(1, &m_uiLineVBO);
	glDeleteBuffers(2, m_uiOBJModelBuffer);
	glDeleteVertexArrays(1, &m_uiLinesVAO);
	glDeleteVertexArrays(1, &m_uiOBJModelVAO);
	ShaderUtilities::DeleteProgram(m_uiOBJProgram);
	ShaderUtilities::DeleteProgram(m_uiProgram);
	TextureManager::DestroyInstance();
	ShaderUtilities::DestroyInstance();
}