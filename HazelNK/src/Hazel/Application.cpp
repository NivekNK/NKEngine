#include "nkpch.h"

#include "Application.h"

#include <glad/gl.h>

namespace nk
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		NK_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

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

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		m_Dispatcher.Dispatch<WindowCloseEvent>(event);

		//DebugCoreInfo(event);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			if (event.Handled)
				break;

			(*--it)->OnEvent(event);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}
