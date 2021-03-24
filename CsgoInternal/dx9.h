#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#define SCREENCENTER Vec2{windowWidth/2.f,windowHeight/2.f}
#define SCENTER_X windowWidth/2.f
#define SCENTER_Y windowWidth/2.f

typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

BOOL GetDeviceVTable(LPVOID pTable, size_t size);

static HWND window;

extern DWORD windowHeight, windowWidth;
extern LPDIRECT3DDEVICE9 pDevice;

