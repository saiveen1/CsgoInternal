#pragma once

#include <windows.h>
#include <d3dx9.h>
#include <memory>//unique_ptr

#include "GameData.h"


DWORD WINAPI HackThread(HMODULE hModule);
VOID APIENTRY HookEndScene(LPDIRECT3DDEVICE9 oldPDevice);

class Hack
{

private:
	LPVOID clientDLL;
	LPVOID engineDLL;
	CBaseEntityList* entList;

	VOID getPlayers();
public:
	std::vector<CEnt*> players;

	Hack();

	VOID Init();
	VOID Dump();
	VOID Run();
	VOID Update();
};
inline std::unique_ptr<Hack> hack;

