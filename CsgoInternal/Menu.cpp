#include "Menu.h"

#include "utils.h"
#include "render.h"

#include <atlstr.h>
Vec2 menuPos{ 300,300 };

VOID Menu::RenderMenu()
{
	if (menu->settings.showMenu)
	{
		render->DrawText(L"Ins Show menu", menuPos, menu->color.optionEnable);
		render->DrawText(L"F1 Teammate", menuPos + 16, menu->settings.visual.showTeamates ? menu->color.optionEnable : menu->color.optionDisable);
		render->DrawText(L"F2 SnapLines", menuPos + 32, menu->settings.visual.snaplines ? menu->color.optionEnable : menu->color.optionDisable);
		render->DrawText(L"F3 Box2D", menuPos + 48, menu->settings.visual.rect ? menu->color.optionEnable : menu->color.optionDisable);
		CString strTemp;
		strTemp.Format(L"Ctrl+9 HealthBar %d", menu->settings.visual.healthBar);
		render->DrawText(strTemp, menuPos + 64, menu->settings.visual.healthBar ? menu->color.optionEnable : menu->color.optionDisable);
		render->DrawText(L"F6 Skeleton", menuPos + 80, menu->settings.visual.skeletonEsp ? menu->color.optionEnable : menu->color.optionDisable);
		render->DrawText(L"F7 NoRecoil", menuPos + 96, menu->settings.noRecoil ? menu->color.optionEnable : menu->color.optionDisable);
		render->DrawText(L"F8 Aimbot", menuPos + 112, menu->settings.aimbot ? menu->color.optionEnable : menu->color.optionDisable);
		render->DrawText(L"Shift aim at the head", menuPos + 128, WHITE);
		render->DrawText(L"-----------------------", menuPos + 144, WHITE);
		render->DrawText(L"Ctrl+0 RecoilCrosshair", menuPos + 160, menu->settings.recoilCrosshair ? menu->color.optionEnable : menu->color.optionDisable);
		render->DrawText(L"ATL + ¡ü¡ýRange¡û¡úSpeed", menuPos + 176, menu->settings.showAimbotRange ? menu->color.optionEnable : menu->color.optionDisable);
		render->DrawText(L"End Quit", menuPos + 190, menu->color.optionDisable);
	}
}

VOID Menu::CheckButtons() {
	if (KEY_PRESSED(buttons.showMenu))
		settings.showMenu = !settings.showMenu;
	if (KEY_PRESSED(buttons.snaplines))
		settings.visual.snaplines = !settings.visual.snaplines;
	if (KEY_PRESSED(buttons.skeletonEsp))
		settings.visual.skeletonEsp = !settings.visual.skeletonEsp;
	if (KEY_PRESSED(buttons.showTeamates))
		settings.visual.showTeamates = !settings.visual.showTeamates;
	if (KEY_PRESSED(buttons.noRecoil))
		settings.noRecoil = !settings.noRecoil;
	if (KEY_PRESSED(buttons.aimbot))
		settings.aimbot = !settings.aimbot;
	if (KEY_PRESSED(buttons.rect))
		settings.visual.rect = !settings.visual.rect;
	if (KEY_PRESSING(VK_LCONTROL))
	{
		if (KEY_PRESSED(buttons.hpEsp))
			settings.visual.healthBar = ((settings.visual.healthBar == 3) ? 0 : settings.visual.healthBar + 1);
		if (KEY_PRESSED(buttons.recoilCrosshair))
			settings.recoilCrosshair = !settings.recoilCrosshair;
	}
}



