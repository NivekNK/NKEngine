#pragma once

#ifdef NK_PLATFORM_WINDOWS
	#ifdef NK_BUILD_DLL
		#define NK_API __declspec(dllexport)
	#else
		#define NK_API __declspec(dllimport)
	#endif
#else
	#error NKEngine only supports Windows!
#endif

#define BIT(x) (1 << x)
