#ifndef RENDERER_H
#define RENDERER_H

// Includes.
#include "Application.h"
#include "GLAD/glad.h"
#include "glm/glm.hpp"

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

	// Vertex buffer to hold our line data for the grid.
	unsigned int m_uiLineVBO;
	unsigned int m_uiOBJProgram;
	unsigned int m_uiOBJModelBuffer[2];

	DebugCamera* m_pDebugCamera;
	OBJModel* m_pOBJModel;
	Line* m_pLines;
};

#endif // RENDERER_H.