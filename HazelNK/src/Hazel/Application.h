#pragma once

#include "Hazel/Core.h"

#include "Hazel/Window.h"

namespace nk
{
	class NK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	/**
	 * \brief Defined in client
	 */
	Application* CreateApplication();
}
