//////////////////////////////
/// File: Skybox.cpp.
/// Author: Liam Bansal.
/// Date Created: 05/01/2021.
//////////////////////////////

#include "Skybox.h" // File's header.
#include "Cubemap.h"
#ifdef WIN64
#include "GLAD/glad.h"
#endif // WIN64.
#include "GLM/glm.hpp"
#include "Renderer.h"

Skybox::Skybox(Renderer* a_parentRenderer) : m_pParentRenderer(a_parentRenderer),
m_pDebugCamera(m_pParentRenderer->GetCamera())
{
	// Skybox VAO & VBO.
	glGenVertexArrays(1, &m_uiSkyboxVAO);
	glGenBuffers(1, &m_uiSkyboxVBO);
	glBindVertexArray(m_uiSkyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiSkyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_fSkyboxVertices), &m_fSkyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Load skybox face textures.
	m_pCubemap = new Cubemap();
	m_uiCubemapTexture = m_pCubemap->Load(m_skyboxFaces);

	// TODO: move into renderer.
	// Configure skybox shader.
	unsigned int skyboxVertexShader = ShaderUtilities::LoadShader("Resources/Shaders/skybox_vertex.glsl",
		GL_VERTEX_SHADER);
	unsigned int skyboxFragmentShader = ShaderUtilities::LoadShader("Resources/Shaders/skybox_fragment.glsl",
		GL_FRAGMENT_SHADER);
	// Create a shader program using the skybox shader files.
	unsigned int m_uiSkyboxProgram = ShaderUtilities::CreateProgram(skyboxVertexShader, skyboxFragmentShader);;
	// Set program to the skybox shader ID.
	SetProgram(m_uiSkyboxProgram);
	int skyboxUniformLocation = glGetUniformLocation(m_uiSkyboxProgram, "skybox");
	glUniform1i(skyboxUniformLocation, 3);
}

Skybox::~Skybox()
{
	// TODO: deallocate memory for cubemap texture.
	glDeleteVertexArrays(1, &m_uiSkyboxVAO);
	glDeleteBuffers(1, &m_uiSkyboxVAO);
}

// TODO: finish converting skybox tutorial code into something usable for this project.
void Skybox::Draw()
{
	// Change depth function so depth test passes when values are equal to 
	// depth buffer's content.
	glDepthFunc(GL_LEQUAL);
	SetProgram(m_uiSkyboxProgram);
	// TODO: set the actual camera matrix I'm using and not this useless variable.
	// Remove translation from the view matrix.
	// Camera view matrix. This is the DebugCamera m_projectionViewMatrix.
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	//skyboxShader.setMat4("view", view);
	// ID = Shader ID.
	// mat / view = Camera view matrix.
	// Was: glUniformMatrix4fv(glGetUniformLocation(ID, "view".c_str()), 1, GL_FALSE, &mat[0][0]);
	// I think I only have a camera projection matrix.
	// This is the camera view that we need to set to the cameras forward position.
	glUniformMatrix4fv(glGetUniformLocation(m_pParentRenderer->GetProgram(),
		"cameraPosition"), // or it's the camera projection.
		1,
		GL_FALSE,
		&mat[0][0]);
	//skyboxShader.setMat4("projection", projection);
	// ID = Shader ID.
	// projection / mat = Camera projection matrix.
	int projectionViewUniformLocation =
		glGetUniformLocation(m_pParentRenderer->GetProgram(), "projectionViewMatrix");
	glUniformMatrix4fv(projectionViewUniformLocation,
		1,
		GL_FALSE,
		&mat[0][0]);
	// Skybox cube.
	glBindVertexArray(m_uiSkyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiCubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	// Set depth function back to default.
	glDepthFunc(GL_LEQUAL);
}