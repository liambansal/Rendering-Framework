#ifndef APPLICATION_H
#define APPLICATION_H

// Forward declarations.
struct GLFWwindow;

class Application
{
public:
	Application();
	virtual ~Application();

	bool Create(const char* a_applicationName,
		unsigned int a_windowWidth,
		unsigned int a_windowheight,
		bool fullscreen);
	void Run(const char* a_application,
		unsigned int a_windowWidth,
		unsigned int a_windowHeight,
		bool a_fullscreen);
	void Quit();

protected:
	unsigned int m_uiWindowWidth;
	unsigned int m_uiWindowHeight;

private:
	// Pure virtual function to be implemented by child classes.
	virtual bool OnCreate() = 0;
	virtual void Update(float a_deltaTime) = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;

	bool m_bRunning;
	GLFWwindow* m_pWindow;
};

#endif // APPLICATION_H.