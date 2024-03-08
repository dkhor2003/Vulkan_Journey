#ifndef __MY_WINDOW_H__
#define __MY_WINDOW_H__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <string>

class MyApplication;

class MyWindow
{
public:
	MyWindow(int w, int h, std::string name);
	~MyWindow();

	// Cannot do copy contrucror or assignment
	// because we have a GLGWwindow pointer
	MyWindow(const MyWindow&) = delete;
	MyWindow& operator=(const MyWindow&) = delete;

	bool       shouldClose()      { return glfwWindowShouldClose(m_pWindow); }
	VkExtent2D extent()           { return { static_cast<uint32_t>(m_iWidth), static_cast<uint32_t>(m_iHeight) }; };
	void       createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
	void       pollEvents();
	void 	   ResetView(); 
	void       ChangeColor(); 

	// Application specific functions
	void       bindMyApplication(MyApplication* pMyApplication);
	void       setLButtonPress(float x, float y);

private:
	static void s_keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void s_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void _initWindow();

	const int      m_iWidth;
	const int      m_iHeight;

	std::string    m_sWindowName;
	GLFWwindow*    m_pWindow;

	// Application specific member data
	MyApplication* m_pMyApplication;
};

#endif

