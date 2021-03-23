#include "Hook.h"


namespace Hook
{
	VOID Patch(LPVOID dst, LPVOID src, size_t size)
	{
		DWORD oldProt = 0;
		::VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProt);
		memcpy(dst, src, size);
		::VirtualProtect(dst, size, oldProt, &oldProt);
	}

	BOOL Hook(LPVOID origin, LPVOID dst, size_t len)
	{
		if (len < 5)
			return FALSE;

		DWORD oldProt = 0;
		::VirtualProtect(origin, len, PAGE_EXECUTE_READWRITE, &oldProt);
		memset(origin, 0x90, len);
		auto jmpAddr = (UINT_PTR)dst - (UINT_PTR)origin - 5;
		*(BYTE*)origin = 0xE9;
		*(UINT_PTR*)((UINT_PTR)origin + 1) = jmpAddr;
		::VirtualProtect(origin, len, oldProt, &oldProt);
		return TRUE;
	}

	LPVOID TrampHook(LPVOID src, LPVOID dst, size_t len)
	{
		if (len < 5)
			return NULL;

		LPVOID pGateWay = ::VirtualAlloc(NULL, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		if (!pGateWay)
			return NULL;
		memcpy(pGateWay, src, len);
		auto jmpAddr = (UINT_PTR)src - (UINT_PTR)pGateWay - 5;
		*(BYTE*)((UINT_PTR)pGateWay + len) = 0xE9;
		*(UINT_PTR*)((UINT_PTR)pGateWay + len + 1) = jmpAddr;

		if (Hook(src, dst, len))
			return pGateWay;
		return NULL;
	}
}