#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE module, DWORD  reason, LPVOID reserved) {
	return TRUE;
}

#endif  // defined(_WIN32)