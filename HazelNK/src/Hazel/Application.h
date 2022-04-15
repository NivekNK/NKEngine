#pragma once

#include "Hazel/Core.h"

#include "Hazel/Window.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace nk
{
	class NK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);
	private:
		bool OnWindowClose(WindowCloseEvent& event);

		EventDispatcher m_Dispatcher;

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	/**
	 * \brief Defined in client
	 */
	Application* CreateApplication();
}
