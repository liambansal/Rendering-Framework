#include "Application.h" // File's header.
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Utilities.h"
#include "ShaderUtilities.h"

Application::Application() : m_uiWindowWidth(0),
	m_uiWindowHeight(0),
	m_bRunning(false),
	m_pWindow(nullptr)
{}

Application::~Application()
{}

bool Application::Create(const char* a_applicationName,
	unsigned int a_windowWidth,
	unsigned int a_windowHeight,
	bool a_fullscreen)
{
	unsigned int majorVersion = 4;
	unsigned int minorVersion = 6;
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	if (!glfwInit())
	{
		return false;
	}

	m_uiWindowWidth = a_windowWidth;
	m_uiWindowHeight = a_windowHeight;
	// Create windowed move window and it's OpenGL context.
	m_pWindow = glfwCreateWindow(m_uiWindowWidth,
		m_uiWindowHeight,
		a_applicationName,
		a_fullscreen ? glfwGetPrimaryMonitor() : nullptr,
		nullptr);

	if (!m_pWindow)
	{
		std::cout << "Failed to create GLFW window./n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_pWindow);

	if (!gladLoadGL())
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}

	majorVersion = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_VERSION_MAJOR);
	minorVersion = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_VERSION_MINOR);
	unsigned int revision = glfwGetWindowAttrib(m_pWindow,
		GLFW_CONTEXT_REVISION);
	// Output window attributes to inform user of OpenGL version.
	std::cout << "OpenGL version: " << majorVersion << "." << minorVersion << "." << revision << std::endl;
	// Implement a call to the derived class onCreate function for any implementation specific code.
	bool result = OnCreate();

	if (!result)
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	return result;
}

void Application::Run(const char* a_application,
	unsigned int a_windowWidth,
	unsigned int a_windowHeight,
	bool a_fullscreen)
{
	if (Create(a_application, a_windowWidth, a_windowHeight, a_fullscreen))
	{
		Utilities::ResetTimer();
		m_bRunning = true;

		do
		{
			float deltaTime = Utilities::TickTimer();
			Update(deltaTime);
			Draw();
			// Updates the buffer used to render images to the screen.
			glfwSwapBuffers(m_pWindow);
			// Checks triggered events e.g. keyboard input.
			glfwPollEvents();
		} while (m_bRunning == true && glfwWindowShouldClose(m_pWindow) == 0);
		
		Destroy();
	}

	ShaderUtilities::DestroyInstance();
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void Application::Quit()
{
	m_bRunning = false;
}