// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>

#include "hack.h"


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
		if (!hThread)
			return FALSE;
		::CloseHandle(hThread);
		break;
	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

