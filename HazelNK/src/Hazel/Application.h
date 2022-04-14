#pragma once

#include "Hazel/Core.h"

namespace nk
{
	class NK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	/**
	 * \brief Defined in client
	 */
	Application* CreateApplication();
}
