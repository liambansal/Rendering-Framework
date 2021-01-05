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
	const OBJModel* GetModel() const;
	DebugCamera* GetCamera() const;

protected:
	virtual bool OnCreate();
	virtual void Update(float a_deltaTime);
	virtual void Draw();
	virtual void Destroy();

	GLuint m_uiProgram;

private:
	void SetProgram(unsigned int program);

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

	/// <summary>
	/// Vertex buffer to hold our line data for the grid.
	/// </summary>
	unsigned int m_uiLineVBO;
	unsigned int m_uiLinesVAO;
	unsigned int m_uiOBJProgram;
	/// <summary>
	/// Variable to keep track of currenly bound shader program
	/// </summary>
	unsigned int m_currentProgram;
	unsigned int m_uiOBJModelVAO;
	unsigned int m_uiOBJModelBuffer[2];

	DebugCamera* m_pDebugCamera;
	OBJModel* m_pOBJModel;
	Line* m_pLines;
};

#endif // RENDERER_H.