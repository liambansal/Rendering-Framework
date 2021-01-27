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

Skybox::Skybox(Renderer* a_parentRenderer) : m_uiCubemapTexture(0),
	m_poCubemap(nullptr),
	m_poParentRenderer(a_parentRenderer)
{
	m_poDebugCamera = m_poParentRenderer->GetCamera();
	// Skybox VAO & VBO.
	//glGenVertexArrays(1, &m_uiSkyboxVAO);
	//glGenBuffers(1, &m_uiSkyboxVBO);
	//glBindVertexArray(m_uiSkyboxVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, m_uiSkyboxVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(m_fSkyboxVertices), &m_fSkyboxVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// Load skybox face textures.
	//m_poCubemap = new Cubemap();
	//m_uiCubemapTexture = m_poCubemap->Load(m_skyboxFaces);
}

Skybox::~Skybox()
{
	if (m_poCubemap)
	{
		delete m_poCubemap;
		m_poCubemap = nullptr;
	}

	glDeleteTextures(1, &m_uiCubemapTexture);
	glDeleteVertexArrays(1, &m_uiSkyboxVAO);
	glDeleteBuffers(1, &m_uiSkyboxVAO);
}

unsigned int Skybox::GetTexture() const
{
	return m_uiCubemapTexture;
}

unsigned int Skybox::GetVAO() const
{
	return m_uiSkyboxVAO;
}
