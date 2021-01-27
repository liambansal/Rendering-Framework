//////////////////////////////
// File: Renderer.h.
// Author: Liam Bansal.
// Date Created: 26/09/2020.
//////////////////////////////

#ifndef RENDERER_H
#define RENDERER_H

// Includes.
#include "Application.h"
#ifdef WIN64
#include "GLAD/glad.h"
#endif // WIN64.
#include "GLM/glm.hpp"
#ifdef NX64
#include <nn/nn_Log.h>
#include <nn/gll.h>
#endif // NX64.

// Forward Declarations.
class DebugCamera;
class OBJModel;
class Skybox;

class Renderer : public Application
{
public:
	// Constructor.
	Renderer();
	// Destructor.
	~Renderer();

	const unsigned int GetWindowWidth() const;
	const unsigned int GetWindowHeight() const;
	const GLuint GetProgram() const;
	const OBJModel* GetModel(unsigned int a_model) const;
	const unsigned int GetNumberOfModels() const;
	DebugCamera* GetCamera() const;

protected:
	virtual bool OnCreate();
	virtual void Update(float a_deltaTime);
	virtual void Draw();
	virtual void Destroy();

	GLuint m_uiProgram;

private:
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

	void SetProgram(unsigned int a_program);

	unsigned int m_uiNumberOfModels;
	/// <summary>
	/// Vertex buffer to hold our line data for the grid.
	/// </summary>
	unsigned int m_uiLineVBO;
	unsigned int m_uiLinesVAO;
	unsigned int m_uiOBJProgram;
	unsigned int m_uiSkyboxProgram;
	/// <summary>
	/// Variable to keep track of currently bound shader program
	/// </summary>
	unsigned int m_uiCurrentProgram;
	unsigned int m_uiOBJModelVAO;
	unsigned int m_uiOBJModelBuffer[2];

	DebugCamera* m_poDebugCamera;
	OBJModel* m_poOBJModels[2];
	Line* m_pLines;
	Skybox* m_poSkybox;
};

#endif // RENDERER_H.