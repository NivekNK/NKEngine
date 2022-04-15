#include "nkpch.h"

#include "WindowsWindowOpenGL.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

namespace nk
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		DebugCoreError("[GLFW Error: {}] {}", error, description);
	}

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
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height), m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, const int width, const int height)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, const int keycode, int scanCode, int action, const int mods)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(keycode, mods, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(keycode, mods);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(keycode, mods, 1);
					data.EventCallback(event);
					break;
				}
				default:
				{
					DebugCoreWarning("Key event performed with action [{}] is not implemented!", action);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			KeyTypedEvent event(keycode, -1);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, const int button, int action, int mods)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				default:
				{
					DebugCoreWarning("Mouse event performed with action [{}] is not implemented!", action);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, const double xOffset, const double yOffset)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseScrolledEvent event((xOffset), (yOffset));
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, const double xPos, const double yPos)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseMovedEvent event((xPos), (yPos));
			data.EventCallback(event);
		});
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