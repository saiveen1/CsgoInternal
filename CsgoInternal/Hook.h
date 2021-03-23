#pragma once
#include <windows.h>
namespace Hook
{
	VOID Patch(LPVOID dst, LPVOID src, size_t size);

	BOOL Hook(LPVOID origin, LPVOID dst, size_t len);

	LPVOID TrampHook(LPVOID src, LPVOID dst, size_t len);
};

