#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

// Includes.
#include "Renderer.h"
#ifdef NX64
#include <nn/nn_Macro.h>
#endif // NX64.

class DebugCamera : public Renderer
{
public:
	// Constructor.
	DebugCamera(Renderer* a_parentRenderer);
	// Destructor.
	~DebugCamera();

	// Utility for mouse/keyboard movement of a matrix transform (suitable for
	// camera).
	void FreeMovement(float a_deltaTime,
		const glm::vec3& a_up = glm::vec3(0, 1, 0));
	void UpdateProjectionView();
	void UpdateCameraPosition();
	glm::mat4 GetCameraMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

private:
#ifdef WIN64
	const float m_fCameraSpeed;
	const float m_fSpeedMultiplier;
#endif // WIN64.
#ifdef NX64
	const float NN_IS_UNUSED_MEMBER(m_fCameraSpeed);
	const float NN_IS_UNUSED_MEMBER(m_fSpeedMultiplier);
#endif // NX64.
	// World space matrix for the camera.
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_projectionViewMatrix;
	Renderer* m_pParentRenderer;
};

#endif // DEBUG_CAMERA_H.