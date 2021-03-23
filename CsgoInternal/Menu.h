#pragma once
#include <windows.h>
#include <d3d9.h>
#include <memory>
class Menu
{
public:
	//inline Menu();
	VOID CheckButtons();

public:
	struct Settings {
		BOOL showMenu = TRUE;
		BOOL showTeamates = FALSE;
		BOOL snaplines = FALSE;
		BOOL rect = FALSE;
		UINT hpEsp = 0;
		BOOL hpText = FALSE;
		BOOL skeletonEsp = FALSE;
		BOOL aimbot = FALSE;
		BOOL fixedCrosshair = FALSE;
	}settings;

	struct Buttons {
		UINT showMenu = VK_INSERT;
		UINT showTeamates = VK_F1;
		UINT snaplines = VK_F2;
		UINT rect = VK_F3;
		UINT hpEsp = VK_F4;
		UINT hpText = VK_F5;
		UINT skeletonEsp = VK_F6;
		UINT aimbot = VK_F7;
		UINT startAim = VK_LSHIFT;
		UINT fixedCrosshair = 0x30; //ctrl
		UINT exit = VK_END;
	}buttons;

	struct Colors {
		D3DCOLOR optionEnable = D3DCOLOR_ARGB(255, 255, 255, 0);
		D3DCOLOR optionDisable = D3DCOLOR_ARGB(255, 255, 255, 255);
		D3DCOLOR crosshair = D3DCOLOR_ARGB(255, 0, 255, 0);
		struct Team {
			D3DCOLOR esp = D3DCOLOR_ARGB(255, 0, 255, 0);
			D3DCOLOR snapline = D3DCOLOR_ARGB(255, 0, 255, 0);
			D3DCOLOR healthBar = D3DCOLOR_ARGB(255, 255, 0, 255);
			D3DCOLOR healthCircle = D3DCOLOR_ARGB(255, 0, 255, 100);
			D3DCOLOR skeleton = D3DCOLOR_ARGB(255, 0, 255, 100);
		}team;
		struct Enemy {
			D3DCOLOR esp = D3DCOLOR_ARGB(255, 255, 0, 0);
			D3DCOLOR snapline = D3DCOLOR_ARGB(255, 255, 0, 0);
			D3DCOLOR healthBar = D3DCOLOR_ARGB(255, 255, 0, 255);
			D3DCOLOR healthCircle = D3DCOLOR_ARGB(255, 255, 255, 0);
			D3DCOLOR skeleton = D3DCOLOR_ARGB(255, 255, 255, 0);
		}enemy;
	}color;
};

inline std::unique_ptr<Menu> menu;

