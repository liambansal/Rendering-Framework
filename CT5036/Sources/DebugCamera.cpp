#include "DebugCamera.h" // File's header.
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"

DebugCamera::DebugCamera() : m_cameraMatrix(glm::inverse(
		glm::lookAt(glm::vec3(10, 10, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)))),
	m_projectionMatrix(glm::perspective(
		glm::pi<float>() * 0.25f,
		(float)1080 / (float)1920,
		0.1f,
		1000.0f)),
	m_projectionViewMatrix(NULL)
{}

DebugCamera::~DebugCamera()
{}

// Utility for mouse/keyboard movement of a matrix transform (suitable for 
// camera).
void DebugCamera::FreeMovement(float a_deltaTime,
	float a_speed
	/*const glm::vec3& a_up = glm::vec3(0, 1, 0)*/)
{
	// Get the current window context.
	GLFWwindow* window = glfwGetCurrentContext();
	// Get the camera's forward, right, up and location vectors.
	glm::vec4 vForward = m_cameraMatrix[2];
	glm::vec4 vRight = m_cameraMatrix[0];
	glm::vec4 vUp = m_cameraMatrix[1];
	glm::vec4 vTranslation = m_cameraMatrix[3];
	const float speedMultiplier = 2.0f;
	// Test to see if the left shift key is pressed. We will use left shift to 
	// the speed of the camera movement.
	float frameSpeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ?
		a_deltaTime * a_speed * speedMultiplier : a_deltaTime * a_speed;

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

		// Pitch.
		if (iDeltaY != 0)
		{
			const float number = 150.0f;
			mMat = glm::axisAngleMatrix(vRight.xyz(), (float)-iDeltaY / number);
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
}

void DebugCamera::UpdateProjectionView()
{
	// Send the projection matrix to the vertex shader.
	// Ask the shader program for the location of the projection-view-
	// matrix uniform variable.
	int projectionViewUniformLocation = glGetUniformLocation(3, "projectionViewMatrix");
	// Send this location a pointer to our glm::mat4 (send across float data).
	glUniformMatrix4fv(projectionViewUniformLocation,
		1,
		false,
		glm::value_ptr(m_projectionViewMatrix));
}

void DebugCamera::SendShaderData()
{
	glm::mat4 viewMatrix = glm::inverse(m_cameraMatrix);
	m_projectionViewMatrix = m_projectionMatrix * viewMatrix;
}
