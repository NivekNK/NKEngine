#include "nkpch.h"

#include "WindowsWindowOpenGL.h"

namespace nk
{
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindowOpenGL(props);
	}

	WindowsWindowOpenGL::WindowsWindowOpenGL(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindowOpenGL::~WindowsWindowOpenGL()
	{
		Shutdown();
	}

	void WindowsWindowOpenGL::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		DebugCoreInfo("Creating window {} ({}, {})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			//TODO: glfwTerminate on system shutdown
			const int success = glfwInit();
			NK_CORE_ASSERT(success, "Could not initialize GLFW!")

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height), m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindowOpenGL::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindowOpenGL::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindowOpenGL::SetVSync(const bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindowOpenGL::IsVSync() const
	{
		return m_Data.VSync;
	}
}