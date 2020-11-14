#ifndef RENDERER_H
#define RENDERER_H

// Includes.
#include "Application.h"
#include "GLAD/glad.h"
#include "GLM/glm.hpp"

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
	unsigned int m_uiOBJProgram;
	/// <summary>
	/// Variable to keep track of currenly bound shader program
	/// </summary>
	unsigned int m_currentProgram;
	unsigned int m_uiOBJModelBuffer[2];

	DebugCamera* m_pDebugCamera;
	OBJModel* m_pOBJModel;
	Line* m_pLines;
};

#endif // RENDERER_H.