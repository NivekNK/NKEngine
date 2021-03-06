#include "nkpch.h"

#include "ImGuiLayer.h"

#include "Hazel/Core.h"
#include "Hazel/Application.h"

#ifdef NK_OPENGL
	#include "Platform/OpenGL/imgui_impl_opengl3.h"
#elif defined(NK_VULKAN)
	#include "Platform/Vulkan/imgui_impl_vulkan.h"
#endif

// TEMPORARY
#ifdef NK_OPENGL
	#include <glad/gl.h>
#elif defined(NK_VULKAN)
	#include "vulkan/vulkan.hpp"
#endif
#include <GLFW/glfw3.h>

namespace nk
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
		m_Dispatcher.AddListener<MouseButtonPressedEvent>(NK_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		m_Dispatcher.AddListener<MouseButtonReleasedEvent>(NK_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		m_Dispatcher.AddListener<MouseMovedEvent>(NK_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		m_Dispatcher.AddListener<MouseScrolledEvent>(NK_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		m_Dispatcher.AddListener<KeyPressedEvent>(NK_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		m_Dispatcher.AddListener<KeyReleasedEvent>(NK_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		m_Dispatcher.AddListener<KeyTypedEvent>(NK_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		m_Dispatcher.AddListener<WindowResizeEvent>(NK_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	ImGuiLayer::~ImGuiLayer() = default;

	void ImGuiLayer::OnAttach()
	{
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

#ifdef NK_OPENGL
        ImGui_ImplOpenGL3_Init("#version 410");
#elif defined(NK_VULKAN)
		ImGui_ImplVulkan_InitInfo vulkanInfo{};
		ImGui_ImplVulkan_Init(&vulkanInfo, );
#endif
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
        ImGuiIO& io = ImGui::GetIO();
        const Window& window = Application::Get().GetWindow();
        io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());

        const auto time = static_cast<float>(glfwGetTime());
        io.DeltaTime = m_Time > 0.0f ? time - m_Time : 1.0f / 60.0f;
        m_Time = time;

#ifdef NK_OPENGL
        ImGui_ImplOpenGL3_NewFrame();
#elif defined(NK_VULKAN)
		ImGui_ImplVulkan_NewFrame();
#endif
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
#ifdef NK_OPENGL
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#elif defined(NK_VULKAN)
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), );
#endif
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		m_Dispatcher.Dispatch<MouseButtonPressedEvent>(event);
		m_Dispatcher.Dispatch<MouseButtonReleasedEvent>(event);
		m_Dispatcher.Dispatch<MouseMovedEvent>(event);
		m_Dispatcher.Dispatch<MouseScrolledEvent>(event);
		m_Dispatcher.Dispatch<KeyPressedEvent>(event);
		m_Dispatcher.Dispatch<KeyReleasedEvent>(event);
		m_Dispatcher.Dispatch<KeyTypedEvent>(event);
		m_Dispatcher.Dispatch<WindowResizeEvent>(event);
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(event.GetX(), event.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += event.GetXOffset();
		io.MouseWheel += event.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.AddKeyEvent(ImGuiKey_ModCtrl, (event.GetMods() & GLFW_MOD_CONTROL) != 0);
		io.AddKeyEvent(ImGuiKey_ModShift, (event.GetMods() & GLFW_MOD_SHIFT) != 0);
		io.AddKeyEvent(ImGuiKey_ModAlt, (event.GetMods() & GLFW_MOD_ALT) != 0);
		io.AddKeyEvent(ImGuiKey_ModSuper, (event.GetMods() & GLFW_MOD_SUPER) != 0);

		const ImGuiKey imGuiKey = ImGui_ImplGlfw_KeyToImGuiKey(event.GetKeyCode());
		io.AddKeyEvent(imGuiKey, true);

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		const ImGuiKey imGuiKey = ImGui_ImplGlfw_KeyToImGuiKey(event.GetKeyCode());
		io.AddKeyEvent(imGuiKey, false);

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (const int keycode = event.GetKeyCode(); keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter(static_cast<unsigned>(keycode));

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, event.GetWidth(), event.GetHeight());

		return false;
	}

    // HACK: Change to NK Key Codes
    ImGuiKey ImGuiLayer::ImGui_ImplGlfw_KeyToImGuiKey(const int key)
    {
        switch (key)
        {
	        case GLFW_KEY_TAB: return ImGuiKey_Tab;
	        case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
	        case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
	        case GLFW_KEY_UP: return ImGuiKey_UpArrow;
	        case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
	        case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
	        case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
	        case GLFW_KEY_HOME: return ImGuiKey_Home;
	        case GLFW_KEY_END: return ImGuiKey_End;
	        case GLFW_KEY_INSERT: return ImGuiKey_Insert;
	        case GLFW_KEY_DELETE: return ImGuiKey_Delete;
	        case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
	        case GLFW_KEY_SPACE: return ImGuiKey_Space;
	        case GLFW_KEY_ENTER: return ImGuiKey_Enter;
	        case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
	        case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
	        case GLFW_KEY_COMMA: return ImGuiKey_Comma;
	        case GLFW_KEY_MINUS: return ImGuiKey_Minus;
	        case GLFW_KEY_PERIOD: return ImGuiKey_Period;
	        case GLFW_KEY_SLASH: return ImGuiKey_Slash;
	        case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
	        case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
	        case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
	        case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
	        case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
	        case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
	        case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
	        case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
	        case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
	        case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
	        case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
	        case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
	        case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
	        case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
	        case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
	        case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
	        case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
	        case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
	        case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
	        case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
	        case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
	        case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
	        case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
	        case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
	        case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
	        case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
	        case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
	        case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
	        case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
	        case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
	        case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
	        case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
	        case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
	        case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
	        case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
	        case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
	        case GLFW_KEY_MENU: return ImGuiKey_Menu;
	        case GLFW_KEY_0: return ImGuiKey_0;
	        case GLFW_KEY_1: return ImGuiKey_1;
	        case GLFW_KEY_2: return ImGuiKey_2;
	        case GLFW_KEY_3: return ImGuiKey_3;
	        case GLFW_KEY_4: return ImGuiKey_4;
	        case GLFW_KEY_5: return ImGuiKey_5;
	        case GLFW_KEY_6: return ImGuiKey_6;
	        case GLFW_KEY_7: return ImGuiKey_7;
	        case GLFW_KEY_8: return ImGuiKey_8;
	        case GLFW_KEY_9: return ImGuiKey_9;
	        case GLFW_KEY_A: return ImGuiKey_A;
	        case GLFW_KEY_B: return ImGuiKey_B;
	        case GLFW_KEY_C: return ImGuiKey_C;
	        case GLFW_KEY_D: return ImGuiKey_D;
	        case GLFW_KEY_E: return ImGuiKey_E;
	        case GLFW_KEY_F: return ImGuiKey_F;
	        case GLFW_KEY_G: return ImGuiKey_G;
	        case GLFW_KEY_H: return ImGuiKey_H;
	        case GLFW_KEY_I: return ImGuiKey_I;
	        case GLFW_KEY_J: return ImGuiKey_J;
	        case GLFW_KEY_K: return ImGuiKey_K;
	        case GLFW_KEY_L: return ImGuiKey_L;
	        case GLFW_KEY_M: return ImGuiKey_M;
	        case GLFW_KEY_N: return ImGuiKey_N;
	        case GLFW_KEY_O: return ImGuiKey_O;
	        case GLFW_KEY_P: return ImGuiKey_P;
	        case GLFW_KEY_Q: return ImGuiKey_Q;
	        case GLFW_KEY_R: return ImGuiKey_R;
	        case GLFW_KEY_S: return ImGuiKey_S;
	        case GLFW_KEY_T: return ImGuiKey_T;
	        case GLFW_KEY_U: return ImGuiKey_U;
	        case GLFW_KEY_V: return ImGuiKey_V;
	        case GLFW_KEY_W: return ImGuiKey_W;
	        case GLFW_KEY_X: return ImGuiKey_X;
	        case GLFW_KEY_Y: return ImGuiKey_Y;
	        case GLFW_KEY_Z: return ImGuiKey_Z;
	        case GLFW_KEY_F1: return ImGuiKey_F1;
	        case GLFW_KEY_F2: return ImGuiKey_F2;
	        case GLFW_KEY_F3: return ImGuiKey_F3;
	        case GLFW_KEY_F4: return ImGuiKey_F4;
	        case GLFW_KEY_F5: return ImGuiKey_F5;
	        case GLFW_KEY_F6: return ImGuiKey_F6;
	        case GLFW_KEY_F7: return ImGuiKey_F7;
	        case GLFW_KEY_F8: return ImGuiKey_F8;
	        case GLFW_KEY_F9: return ImGuiKey_F9;
	        case GLFW_KEY_F10: return ImGuiKey_F10;
	        case GLFW_KEY_F11: return ImGuiKey_F11;
	        case GLFW_KEY_F12: return ImGuiKey_F12;
	        default: return ImGuiKey_None;
        }
    }
}
