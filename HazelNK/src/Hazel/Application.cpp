#include "Application.h"

#include "Hazel/Log.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace nk
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		const WindowResizeEvent e(1200, 700);
		DebugTrace(e);

		while (true);
	}
}
