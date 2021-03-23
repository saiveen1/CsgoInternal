#pragma once
#include <windows.h>
#include <string.h>
#include <stdio.h>

#include "math.h"

#define KEY_PRESSED(key) (GetAsyncKeyState(key) & 1)
#define KEY_PRESSING(key) (GetAsyncKeyState(key) & 0x8000) == 0x8000 ? 1 : 0
namespace Utils
{
    /*
     * @brief Create console
     *
     * Create and attach a console window to the current process
     */
    void AttachConsole();
    //命令行窗口
    /*
     * @brief Detach console
     *
     * Detach and destroy the attached console
     */
    void DetachConsole();

     /*
     * @brief Print to console
     *
     * Replacement to printf that works with the newly created console
     */
    bool ConsolePrint(const char* fmt, ...);


	template<typename T>
	BOOL IsNULL(T arg);

	template<typename T, typename ... Ts>
	BOOL IsNULL(T arg, Ts ...Targs);

	LPWSTR C2W(LPCSTR szStr);

	//将宽字节wchar_t*转化为单字节char*  
	LPSTR W2C(LPCWSTR szStr);

	FLOAT GetDistance(Vec2 src, Vec2 dst);

	PIMAGE_NT_HEADERS GetNtHeaders(LPVOID pBuffer);

	PIMAGE_SECTION_HEADER GetSectionHeader(LPVOID pModule);

	BYTE* PatternScan(LPVOID pModule, LPCSTR pattern);

};
template<typename T>
BOOL Utils::IsNULL(T arg)
{
	if (!arg)
		return TRUE;
	return FALSE;
}

template<typename T, typename ...Ts>
BOOL Utils::IsNULL(T arg, Ts ...Targs)
{
	if (!arg)
		return TRUE;
	else if (IsNULL(Targs ...))
		return TRUE;
	return FALSE;
}


