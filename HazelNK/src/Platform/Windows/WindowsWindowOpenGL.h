#pragma once

#include "Hazel/Window.h"

#include <GLFW/glfw3.h>

namespace nk
{
	class WindowsWindowOpenGL : public Window
	{
	public:
		explicit WindowsWindowOpenGL(const WindowProps& props);
		virtual ~WindowsWindowOpenGL() override;

		virtual void OnUpdate() override;

		[[nodiscard]] virtual unsigned int GetWidth() const override { return m_Data.Width; }
		[[nodiscard]] virtual unsigned int GetHeight() const override { return m_Data.Height; }

		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		[[nodiscard]] virtual bool IsVSync() const override;

		[[nodiscard]] virtual void* GetNativeWindow() const override { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		struct WindowData
		{
			std::string Title;
			unsigned int Width{}, Height{};
			bool VSync{};

			EventCallbackFn EventCallback;
		};

		GLFWwindow* m_Window;
		WindowData m_Data;
	};
}