#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>

#include "MinHook.h"

typedef HANDLE(WINAPI* CREATE_FILE_W)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

CREATE_FILE_W p_CreateFileW = nullptr;
CREATE_FILE_W t_CreateFileW;

HANDLE WINAPI h_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (lstrcmpW(lpFileName, L"\\\\.\\ACE-BASE") == 0) {

		uintptr_t address = reinterpret_cast<uintptr_t>(GetModuleHandleA("starrailbase.dll")) + 0xF09A55;

		DWORD oldProtection;
		VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(uint16_t), PAGE_EXECUTE_READWRITE, &oldProtection);
		*reinterpret_cast<uint16_t*>(address) = 0x03EB;
		VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(uint16_t), oldProtection, &oldProtection);

		puts("GG!");

		SuspendThread(GetCurrentThread());
	}

	return p_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {

		AllocConsole();
		freopen("CONOUT$", "w", stdout);

		if (MH_Initialize() != MH_OK)
		{
			printf("MH_Initialize error\n");

			return FALSE;
		}

		if (MH_CreateHookApiEx(L"kernelbase", "CreateFileW", &h_CreateFileW, reinterpret_cast<void**>(&p_CreateFileW), reinterpret_cast<void**>(&t_CreateFileW)) != MH_OK)
		{
			printf("MH_CreateHookApiEx error\n");

			return FALSE;
		}

		if (MH_EnableHook(t_CreateFileW) != MH_OK)
		{
			printf("MH_EnableHook error\n");

			return FALSE;
		}
	}

	return TRUE;
}
