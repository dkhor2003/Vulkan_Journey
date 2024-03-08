#include "my_window.h"
#include "my_application.h"
#include <stdexcept>
#include <iostream>

MyWindow::MyWindow(int w, int h, std::string name) : 
	m_iWidth(w),
	m_iHeight(h),
	m_sWindowName(name),
	m_pWindow(nullptr),
	m_pMyApplication(nullptr)
{
	_initWindow();
}

MyWindow::~MyWindow()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void MyWindow::s_keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto mywindow = reinterpret_cast<MyWindow*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		mywindow->ResetView(); 
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		mywindow->ChangeColor();
	}
}

void MyWindow::s_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		auto mywindow = reinterpret_cast<MyWindow*>(glfwGetWindowUserPointer(window));
		double xpos = 0.0, ypos = 0.0;
		glfwGetCursorPos(window, &xpos, &ypos);

		mywindow->setLButtonPress((float)xpos, (float)ypos);
	}
}

void MyWindow::_initWindow()
{
	glfwInit();

	// Because glfw was designed initially to create OpenGL context by default,
	// by setting GLFW_CLIENT_API to fale, it tells glfw NOT to create OpenGL context 
	// (because we are going to use Vulkan)
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Need to handle the window resizing in a specical way later in Vulkan code
	// so we need to set it to false here for now
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_pWindow = glfwCreateWindow(m_iWidth, m_iHeight, m_sWindowName.c_str(), nullptr, nullptr);

	// For the call back function to use this pointer
	glfwSetWindowUserPointer(m_pWindow, this);

    // Register keyboard callback	
	glfwSetKeyCallback(m_pWindow, s_keyboardCallback);

    // Register mouse button callback
	glfwSetMouseButtonCallback(m_pWindow, s_mouseButtonCallback);
}

void MyWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, m_pWindow, nullptr, surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window");
	}
}

void MyWindow::pollEvents()
{
	glfwPollEvents();
}

void MyWindow::bindMyApplication(MyApplication* pMyApplication)
{
	m_pMyApplication = pMyApplication;
}

void MyWindow::setLButtonPress(float x, float y)
{
	if (m_pMyApplication != nullptr)
	{
		m_pMyApplication->AddPoint(x, y); 
	}
}

void MyWindow::ResetView()
{
	m_pMyApplication->Reset(); 
}

void MyWindow::ChangeColor()
{
	m_pMyApplication->ColorSwitch(); 
}

