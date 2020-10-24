#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

// Includes.
#include "Renderer.h"

class DebugCamera : public Renderer
{
public:
	// Constructor.
	DebugCamera();
	// Destructor.
	~DebugCamera();

	// Utility for mouse/keyboard movement of a matrix transform (suitable for
	// camera).
	void FreeMovement(float a_deltaTime,
		float a_speed
		/*const glm::vec3& a_up = glm::vec3(0, 1, 0)*/);
	void UpdateProjectionView();
	void SendShaderData();

private:
	// World space matrix for the camera.
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_projectionViewMatrix;
};

#endif // DEBUG_CAMERA_H.