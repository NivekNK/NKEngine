#include "nkpch.h"

#include "Application.h"

#include <GLFW/glfw3.h>

namespace nk
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback([this](auto&& arg) { Application::OnEvent(std::forward<decltype(arg)>(arg)); });

		m_Dispatcher.AddListener<WindowCloseEvent>(NK_BIND_EVENT_FN(Application::OnWindowClose));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		DebugCoreInfo(event);

		m_Dispatcher.Dispatch<WindowCloseEvent>(event);
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}
