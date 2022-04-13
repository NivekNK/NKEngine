#include <Hazel.h>

class Sandbox : public nk::Application
{
public:
	Sandbox() = default;
	virtual ~Sandbox() override = default;
};

nk::Application* nk::CreateApplication()
{
	return new Sandbox();
}