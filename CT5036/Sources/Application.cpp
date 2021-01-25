//////////////////////////////
// File: Application.cpp.
// Author: Liam Bansal.
// Date Created: 24/10/2020.
//////////////////////////////

#include "Application.h" // File's header.
#include <iostream>
#ifdef WIN64
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#endif // WIN64.
#include "Utilities.h"
#include "ShaderUtilities.h"

#ifdef NX64
#include <nn/nn_Log.h>
#include <nn/nn_Assert.h>
#include <nn/init.h>
#include <nn/hid.h>
#include <nn/gll.h>
#endif // NX64.

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
#ifdef WIN64
	unsigned int majorVersion = 4;
	unsigned int minorVersion = 6;

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
#endif // WIN64.
#ifdef NX64
	m_uiWindowWidth = 1280;
	m_uiWindowHeight = 720;
	graphicsHelper.Initialize();
	// Initialize OpenGL for Nintendo Switch.
	nngllResult gllResult = nngllInitializeGl();

	if (gllResult != nngllResult_Succeeded)
	{
		graphicsHelper.Finalize();
		return false;
	}

	NN_LOG("GL_VERSION: %s\n", glGetString(GL_VERSION));
	NN_LOG("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	NN_LOG("\nSupported Extensions\n");
	// Check whether the function pointer is null to determine whether that 
	// feature is loaded.
	int extensionCount = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);

	for (int idxExtension = 0; idxExtension < extensionCount; ++idxExtension)
	{
		auto pExtensionName = reinterpret_cast<const char*>(::glGetStringi(GL_EXTENSIONS, idxExtension));
		NN_LOG("%s\n", pExtensionName);
	}
#endif // NX64.

	// Implement a call to the derived class onCreate function for any implementation specific code.
	bool result = OnCreate();

	if (!result)
	{
#ifdef WIN64
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
#endif // WIN64.
#ifdef NX64
		graphicsHelper.Finalize();
#endif // NX64.
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
#ifdef WIN64
			// Updates the buffer used to render images to the screen.
			glfwSwapBuffers(m_pWindow);
			// Checks triggered events e.g. keyboard input.
			glfwPollEvents();
		}
		while (m_bRunning == true && glfwWindowShouldClose(m_pWindow) == 0);
#endif // WIN64.
#ifdef NX64
			graphicsHelper.SwapBuffers();
		}
		while (m_bRunning);
#endif // NX64.
		Destroy();
	}

#ifdef WIN64
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
#endif // WIN64.
#ifdef NX64
	graphicsHelper.Finalize();
#endif // NX64.
}

void Application::Quit()
{
	m_bRunning = false;
}