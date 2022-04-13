#pragma once

#ifdef NK_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	const auto app = nk::CreateApplication();
	app->Run();
	delete app;
}

#endif
