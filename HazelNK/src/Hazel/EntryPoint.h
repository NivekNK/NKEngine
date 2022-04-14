#pragma once

#ifdef NK_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	nk::Log::Init();

	DebugCoreTrace("Hello {}!", "World");
	DebugFatal("Hello {}!", "World");

	const auto app = nk::CreateApplication();
	app->Run();
	delete app;
}

#endif
