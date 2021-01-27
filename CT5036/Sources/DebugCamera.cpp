//////////////////////////////
// File: DebugCamera.cpp.
// Author: Liam Bansal.
// Date Created: 05/10/2020.
//////////////////////////////

#include "DebugCamera.h" // File's header.
#include "GLM/ext.hpp"
#include "OBJLoader.h"
#ifdef WIN64
#include "GLFW/glfw3.h"
#endif // WIN64.

#ifdef NX64
#include <nn/nn_Log.h>
#include <nn/gll.h>
#include <nn/nn_TimeSpan.h>
#include <nn/os.h>
#endif // NX64.

DebugCamera::DebugCamera(Renderer* a_parentRenderer) : mc_fCameraSpeed(6.0f),
	mc_fSpeedMultiplier(6.0f),
	m_cameraMatrix(glm::inverse(
		glm::lookAt(glm::vec3(10, 10, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)))),
	m_projectionViewMatrix(NULL),
	m_poParentRenderer(a_parentRenderer)
{
	m_projectionMatrix = glm::perspective(
		glm::pi<float>() * 0.25f,
		(float)m_poParentRenderer->GetWindowWidth() / (float)m_poParentRenderer->GetWindowHeight(),
		0.1f,
		1000.0f);
}

DebugCamera::~DebugCamera()
{}

// Utility for mouse/keyboard movement of a matrix transform.
void DebugCamera::Move(float a_deltaTime, const glm::vec3& a_up)
{
#ifdef WIN64
	// Get the current window context.
	GLFWwindow* window = glfwGetCurrentContext();
	// Get the camera's forward, right, up and location vectors.
	glm::vec4 vRight = m_cameraMatrix[0];
	glm::vec4 vUp = m_cameraMatrix[1];
	glm::vec4 vForward = m_cameraMatrix[2];
	glm::vec4 vTranslation = m_cameraMatrix[3];
	// Test to see if the left shift key is pressed. We will use left shift to 
	// the speed of the camera movement.
	float frameSpeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ?
		a_deltaTime * mc_fCameraSpeed * mc_fSpeedMultiplier : a_deltaTime * mc_fCameraSpeed;

	// Translate camera.
	// Moves the camera forward.
	if (glfwGetKey(window, 'W') == GLFW_PRESS)
	{
		vTranslation -= vForward * frameSpeed;
	}

	// Moves the camera left.
	if (glfwGetKey(window, 'A') == GLFW_PRESS)
	{
		vTranslation -= vRight * frameSpeed;
	}

	// Moves the camera back.
	if (glfwGetKey(window, 'S') == GLFW_PRESS)
	{
		vTranslation += vForward * frameSpeed;
	}

	// Moves the camera right.
	if (glfwGetKey(window, 'D') == GLFW_PRESS)
	{
		vTranslation += vRight * frameSpeed;
	}

	// Moves the camera up.
	if (glfwGetKey(window, 'Q') == GLFW_PRESS)
	{
		vTranslation -= vUp * frameSpeed;
	}

	// Moves the camera down.
	if (glfwGetKey(window, 'E') == GLFW_PRESS)
	{
		vTranslation += vUp * frameSpeed;
	}

	// Set the translation to the camera matrix that has been passed in.
	m_cameraMatrix[3] = vTranslation;

	// Check for camera rotations.
	// Test for mouse button being held/pressed for rotation (button 2).
	static bool sbMouseButtonDown = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		static double siPreviousMouseX = 0;
		static double siPreviousMouseY = 0;

		if (!sbMouseButtonDown)
		{
			sbMouseButtonDown = true;
			glfwGetCursorPos(window, &siPreviousMouseX, &siPreviousMouseY);
		}

		double mouseX = 0;
		double mouseY = 0;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		double iDeltaX = mouseX - siPreviousMouseX;
		double iDeltaY = mouseY - siPreviousMouseY;
		siPreviousMouseX = mouseX;
		siPreviousMouseY = mouseY;
		glm::mat4 mMat;
		const float sensitivityModifier = 170.0f;

		// Pitch.
		if (iDeltaY != 0)
		{
			mMat = glm::axisAngleMatrix(vRight.xyz(), (float)-iDeltaY / sensitivityModifier);
			vRight = mMat * vRight;
			vUp = mMat * vUp;
			vForward = mMat * vForward;
		}

		// Yaw.
		if (iDeltaX != 0)
		{
			mMat = glm::axisAngleMatrix(a_up, (float)-iDeltaX / sensitivityModifier);
			vRight = mMat * vRight;
			vUp = mMat * vUp;
			vForward = mMat * vForward;
		}

		m_cameraMatrix[0] = vRight;
		m_cameraMatrix[1] = vUp;
		m_cameraMatrix[2] = vForward;
	}
	else
	{
		sbMouseButtonDown = false;
	}
#endif // WIN64.
}

void DebugCamera::UpdateProjectionView()
{
	glm::mat4 viewMatrix = glm::inverse(m_cameraMatrix);
	m_projectionViewMatrix = m_projectionMatrix * viewMatrix;
	// Send the projection matrix to the vertex shader.
	// Ask the shader program for the location of the projection-view-
	// matrix uniform variable.
	int projectionViewUniformLocation = glGetUniformLocation(m_poParentRenderer->GetProgram(),
		"projectionViewMatrix");
	const GLsizei elementsToModify = 1;
	// Send this location a pointer to our glm::mat4 (send across float data).
	glUniformMatrix4fv(projectionViewUniformLocation,
		elementsToModify,
		false,
		glm::value_ptr(m_projectionViewMatrix));
}

void DebugCamera::UpdateCameraPosition()
{
	int cameraPositionUniformLocation = glGetUniformLocation(m_poParentRenderer->GetProgram(),
		"cameraPosition");
	const GLsizei elementsToModify = 1;
	glUniform4fv(cameraPositionUniformLocation,
		elementsToModify,
		glm::value_ptr(m_cameraMatrix[3]));
}

glm::mat4 DebugCamera::GetCameraMatrix() const
{
	return m_cameraMatrix;
}

glm::mat4 DebugCamera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}