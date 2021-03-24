#pragma once
#include <windows.h>
#include <d3d9.h>
#include <memory>

enum{
	disabled = 0,
	topFixed,
	topBoxWithRect,
	lineLeft,
};

class Menu
{
public:
	//inline Menu();
	VOID CheckButtons();

public:

	struct Settings {
		BOOL showMenu = TRUE;
		struct Esp {
			BOOL showTeamates = FALSE;
			BOOL snaplines = FALSE;
			BOOL rect = FALSE;
			UINT healthBar = 0;
			BOOL skeletonEsp = FALSE;
		}visual;

		BOOL noRecoil = FALSE;
		BOOL aimbot = FALSE;
		BOOL recoilCrosshair = FALSE;
		BOOL showAimbotRange = FALSE;
		FLOAT crosshairSize = 13.f;
	}settings;

	struct Buttons {
		UINT showMenu = VK_INSERT;
		UINT showTeamates = VK_F1;
		UINT snaplines = VK_F2;
		UINT rect = VK_F3;
		UINT hpEsp = VK_F4;
		UINT skeletonEsp = VK_F6;
		UINT noRecoil = VK_F7;
		UINT aimbot = VK_F8;
		UINT startAim = VK_LBUTTON;
		UINT recoilCrosshair = 0x30; //ctrl + 0
		UINT exit = VK_END;
	}buttons;

	struct Colors {
		D3DCOLOR optionEnable = D3DCOLOR_ARGB(255, 255, 255, 0);
		D3DCOLOR optionDisable = D3DCOLOR_ARGB(255, 0, 255, 255);
		D3DCOLOR crosshair = D3DCOLOR_ARGB(255, 50, 205, 50);
		D3DCOLOR healthBar = D3DCOLOR_ARGB(255, 255, 165, 0);
		struct Team {
			D3DCOLOR esp = D3DCOLOR_ARGB(255, 0, 255, 0);
			D3DCOLOR snapline = D3DCOLOR_ARGB(255, 0, 255, 0);
			D3DCOLOR healthCircle = D3DCOLOR_ARGB(255, 0, 255, 100);
			D3DCOLOR skeleton = D3DCOLOR_ARGB(255, 0, 255, 100);
		}team;
		struct Enemy {
			D3DCOLOR esp = D3DCOLOR_ARGB(255, 255, 0, 0);
			D3DCOLOR snapline = D3DCOLOR_ARGB(255, 255, 0, 0);
			D3DCOLOR healthRect = D3DCOLOR_ARGB(255, 112, 128, 144);
			D3DCOLOR healthCircle = D3DCOLOR_ARGB(255, 255, 255, 0);
			D3DCOLOR skeleton = D3DCOLOR_ARGB(255, 250, 128, 114);
		}enemy;
	}color;

	VOID RenderMenu();
};

inline std::unique_ptr<Menu> menu;

