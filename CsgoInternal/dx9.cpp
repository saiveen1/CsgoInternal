#include "dx9.h"

DWORD windowHeight, windowWidth;

BOOL CALLBACK enumWindow(HWND handle, LPARAM lp)
{
	DWORD pid;
	::GetWindowThreadProcessId(handle, &pid);
	if (::GetCurrentProcessId() != pid)
		return TRUE;

	window = handle;
	return FALSE;
}

HWND GetProcessWindow()
{
	window = NULL;

	::EnumWindows(enumWindow, NULL);

	RECT size = { 0 };
	if (!window)
		return NULL;
	::GetWindowRect(window, &size);
	windowWidth = size.right - size.left;
	windowHeight = size.bottom - size.top;

	windowHeight -= 29;
	windowWidth -= 5;

	return window;
}

BOOL GetDeviceVTable(LPVOID pTable, size_t size)
{
	if (!pTable)
		return FALSE;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return FALSE;

	IDirect3DDevice9* pGetDevice = NULL;

	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	HRESULT deviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pGetDevice);

	if (deviceCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		deviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pGetDevice);

		if (deviceCreated != S_OK)
		{
			pD3D->Release();
			return FALSE;
		}

	}

	memcpy(pTable, *(LPVOID*)pGetDevice, size);
	pGetDevice->Release();
	pD3D->Release();
	return TRUE;
}
