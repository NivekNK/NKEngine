#include <Hazel.h>

class ExampleLayer : public nk::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	virtual void OnUpdate() override
	{
		DebugTrace("ExampleLayer::Update");
	}

	virtual void OnEvent(nk::Event& event) override
	{
		DebugInfo("[ExampleLayer::OnEvent] {}", event);
	}
};

class Sandbox : public nk::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	virtual ~Sandbox() override = default;
};

nk::Application* nk::CreateApplication()
{
	return new Sandbox();
}