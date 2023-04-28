#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>

#include "hook.h"
#include "bypass.h"

BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {

		AllocConsole();

		FILE* file = freopen("CONOUT$", "w", stdout);

		bypass::init();

		hook::init();

		printf("bypass successfully loaded, happy hacking\n");
	}

	return TRUE;
}