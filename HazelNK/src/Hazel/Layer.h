#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace nk
{
	class NK_API Layer
	{
	public:
		explicit Layer(std::string debugName = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;

		EventDispatcher m_Dispatcher;
	};
}
