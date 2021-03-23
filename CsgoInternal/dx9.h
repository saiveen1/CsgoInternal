#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

BOOL GetDeviceVTable(LPVOID pTable, size_t size);

static HWND window;

extern DWORD windowHeight, windowWidth;
extern LPDIRECT3DDEVICE9 pDevice;

