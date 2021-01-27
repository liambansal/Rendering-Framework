//////////////////////////////
// File: DebugCamera.h
// Author: Liam Bansal.
// Date Created: 05/10/2020.
//////////////////////////////

#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

#include "Renderer.h"
#ifdef NX64
#include <nn/nn_Macro.h>
#endif // NX64.

/// <summary>
/// Used to navigate the scene view and update the screen perspective.
/// </summary>
class DebugCamera : public Renderer
{
public:
	// Constructor.
	DebugCamera(Renderer* a_parentRenderer);
	// Destructor.
	~DebugCamera();

	// Utility for mouse/keyboard movement of a matrix transform.
	void Move(float a_deltaTime,
		const glm::vec3& a_up = glm::vec3(0, 1, 0));
	void UpdateProjectionView();
	void UpdateCameraPosition();
	glm::mat4 GetCameraMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

private:
#ifdef WIN64
	const float mc_fCameraSpeed;
	const float mc_fSpeedMultiplier;
#endif // WIN64.
#ifdef NX64
	const float NN_IS_UNUSED_MEMBER(mc_fCameraSpeed);
	const float NN_IS_UNUSED_MEMBER(mc_fSpeedMultiplier);
#endif // NX64.
	// World space matrix for the camera.
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_projectionViewMatrix;
	Renderer* m_poParentRenderer;
};

#endif // DEBUG_CAMERA_H.