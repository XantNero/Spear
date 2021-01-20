#include "../pch.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"

Window::Window(const WindowProps& props)
{
    m_data.Title = props.Title;
    m_data.Height = props.Height;
    m_data.Width = props.Width;
    m_data.WindowPointer = this;

    if (!glfwInit()) {
        assert(false);  
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

    m_Window = glfwCreateWindow(m_data.Width, m_data.Height,
                                 m_data.Title.c_str(), NULL, NULL);
    if (!m_Window) {
        glfwTerminate();
        assert(false);
    }

     glfwMakeContextCurrent(m_Window);

    if(glewInit() != GLEW_OK) {
        assert(false);
    }   

    glfwSetWindowUserPointer(m_Window, &m_data);
    glfwSetWindowSizeCallback(m_Window, WindowSizeCallback);
    glfwSetKeyCallback(m_Window, KeyCallback);
    glfwSetCharCallback(m_Window, CharCallback);
    glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
    glfwSetScrollCallback(m_Window, ScrollCallback);
    glfwSetCursorPosCallback(m_Window, CursorPosCallback);
}

Window::~Window() 
{
    glfwDestroyWindow(m_Window);
}

void Window::onUpdate() 
{
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

unsigned int Window::getMouseButton(unsigned int button) const
{
    return glfwGetMouseButton(m_Window, button); 
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}

void Window::setVSync(bool mode) 
{
    glfwSwapInterval(mode ? 1 : 0);
}

inline void Window::shutDown() 
{
     glfwTerminate(); 
}

void WindowSizeCallback(GLFWwindow* window, int width, int height) 
{
    WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
    data->Width = width;
    data->Height = height;
    if (data->WindowSizeCallback != nullptr)
        data->WindowSizeCallback(data->WindowPointer, width, height);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
    if (data->KeyCallback != nullptr)
        data->KeyCallback(data->WindowPointer, key, scancode, action, mods);
}

void CharCallback(GLFWwindow* window, unsigned int keycode) 
{
    WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
    if (data->CharCallback != nullptr)
        data->CharCallback(data->WindowPointer, keycode);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
    WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
    if (data->MouseButtonCallback != nullptr)
        data->MouseButtonCallback(data->WindowPointer, button, action, mods);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) 
{
    WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
    if (data->ScrollCallback != nullptr)
        data->ScrollCallback(data->WindowPointer, xOffset, yOffset);
}

void CursorPosCallback(GLFWwindow* window, double xPos, double yPos) 
{
    WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
    data->CursorPosX = xPos;
    data->CursorPosY = yPos;
    if (data->CursorPosCallback != nullptr)
        data->CursorPosCallback(data->WindowPointer, xPos, yPos);    
}