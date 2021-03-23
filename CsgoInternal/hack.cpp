#include "hack.h"
#include "dx9.h"
#include "Hook.h"
#include "math.h"
#include "render.h"
#include "utils.h"
#include "menu.h"
#include "GameData.h"

#include "Misc.h"

LPVOID d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene OldEndScene = NULL;
extern LPDIRECT3DDEVICE9 pDevice = NULL;
extern EntData playerData = { 0 };
extern ClientData clientData = { 0 };
extern CEnt* localEnt = NULL;

DWORD WINAPI HackThread(HMODULE hModule)
{
	try
	{
		if (GetDeviceVTable(d3d9Device, sizeof(d3d9Device)))
		{
			memcpy(EndSceneBytes, d3d9Device[42], 7);

			OldEndScene = (tEndScene)Hook::TrampHook(d3d9Device[42], HookEndScene, 7);
		}

#ifdef _DEBUG
		Utils::AttachConsole();
#endif // _DEBUG

		hack = std::make_unique<Hack>();

		while (!KEY_PRESSED(menu->buttons.exit))
		{
			hack->Update();
		}
	}

	catch (const std::exception& e)
	{
		Utils::ConsolePrint("%s\n", e.what());
		Utils::DetachConsole();
		FreeLibraryAndExitThread(hModule, 1);
	}

	Sleep(1000);
	Hook::Patch(d3d9Device[42], EndSceneBytes, 7);

	Utils::DetachConsole();
	::FreeLibraryAndExitThread(hModule, 0);

}

VOID APIENTRY HookEndScene(LPDIRECT3DDEVICE9 oldPDevice)
{
	if (!pDevice)
		pDevice = oldPDevice;

	hack->Run();

	OldEndScene(pDevice);
}


VOID Hack::getPlayers()
{
	for (auto i = 2ul; i < 32; i++)
	{
		CEnt* ent = entList->m_EntPtrArrary[i].pEnt;
		if (ent->isValidEnt())
			players.push_back(ent);
	}
}

Hack::Hack()
{
	Init();
}

VOID Hack::Init()
{
	render = std::make_unique<Render>();
	menu = std::make_unique<Menu>();
	clientDLL = GetModuleHandle(L"client.dll");
	engineDLL = GetModuleHandle(L"engine.dll");
	Dump();
	localEnt = clientData.pLocal;
	entList = clientData.entityList;
}

VOID Hack::Dump()
{

	clientData.clientState = *(UINT**)(Utils::PatternScan(engineDLL, "A1 ?? ?? ?? ?? 8D 48 08 E9") + 1);
	clientData.entityList = *(CBaseEntityList**)(Utils::PatternScan(clientDLL, "56 8B 89 ?? ?? ?? ?? 85 C9 74 1B") + 3);

	//特征码是yaw的找错了减4才是pitch角
	auto viewAnglesOffset = *(UINT*)(Utils::PatternScan(engineDLL, "F3 0F 11 80 ?? ?? ?? ?? D9 46 08 D9 05") + 4) - 4;
	clientData.viewAngles = (Vec3*)(*clientData.clientState + (UINT)viewAnglesOffset);

	clientData.pLocal = *(CEnt**)(Utils::PatternScan(clientDLL, "8B 35 ?? ?? ?? ?? 57 85 F6 74 56") + 2);

	auto clientInfo = *(UINT*)(Utils::PatternScan(clientDLL, "B9 ?? ?? ?? ?? 50 6A 00 6A 03 83 EC 08") + 1);
	auto matrixOffset = *(UINT*)(Utils::PatternScan(clientDLL, "8D 45 80 8D 8F ?? ?? ?? ?? 50 E8") + 5);
	clientData.vpMatrix = (FLOAT*)((UINT)clientInfo + (UINT)matrixOffset);

	Utils::ConsolePrint("EntityList: %x\nLocalPlayer: %x\nViewAngles: %x\nVPMatrix: %x\n", clientData.entityList, clientData.pLocal, clientData.viewAngles, clientData.vpMatrix);

	playerData.dormant = *(UINT*)(Utils::PatternScan(clientDLL, "8B 5D 08 56 8B F1 88 9E ?? ?? ?? ??") + 8);
	playerData.health = *(UINT*)(Utils::PatternScan(clientDLL, "83 B9 ?? ?? ?? ?? 00 7F 2D 8B 01") + 2);
	playerData.teamNum = *(UINT*)(Utils::PatternScan(clientDLL, "CC 8B 89 ?? ?? ?? ?? E9 ?? ?? ?? ?? CC") + 3);
	playerData.vecOrigin = *(UINT*)(Utils::PatternScan(clientDLL, "0A C8 88 0E F3 0F 10 87 ?? ?? ?? ??") + 8);
	playerData.viewAngles = playerData.vecOrigin - 0xc;
	playerData.eyePosDelta = *(UINT*)(Utils::PatternScan(clientDLL, "F3 0F 11 40 04 F3 0F 10 87 ?? ?? ?? ??") + 9);
	playerData.aimPunchAngles = *(UINT*)(Utils::PatternScan(clientDLL, "0F 28 C1 F3 0F 59 86 ?? ?? ?? ?? 8B 45 08") + 7);
	playerData.boneMatrix = *(UINT*)(Utils::PatternScan(clientDLL, "8B 8E ?? ?? ?? ?? 8D 04 7F C1 E0 04") + 2);
	playerData.bonePointInfo = *(UINT*)(Utils::PatternScan(clientDLL, "8B 86 ?? ?? ?? ?? 85 C0 0F 84") + 2);

}

VOID Hack::Run()
{
	Misc::run();
}

VOID Hack::Update()
{
	menu->CheckButtons();
	memcpy_s(viewProjecitonMatrix, sizeof(viewProjecitonMatrix), clientData.vpMatrix, sizeof(viewProjecitonMatrix));
}

