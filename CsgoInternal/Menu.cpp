#include "Menu.h"

#include "utils.h"

VOID Menu::CheckButtons() {
	if (KEY_PRESSED(buttons.showMenu))
		settings.showMenu = !settings.showMenu;
	if (KEY_PRESSED(settings.snaplines))
		settings.snaplines = !settings.snaplines;
	if (KEY_PRESSED(buttons.hpEsp)) {
		settings.hpEsp = ((settings.hpEsp == 5) ? 0 : settings.hpEsp + 1);
	}
	if (KEY_PRESSED(buttons.hpText))
		settings.hpText = !settings.hpText;
	if (KEY_PRESSED(buttons.rect))
		settings.rect = !settings.rect;
	if (KEY_PRESSED(buttons.skeletonEsp))
		settings.skeletonEsp = !settings.skeletonEsp;
	if (KEY_PRESSED(buttons.showTeamates))
		settings.showTeamates = !settings.showTeamates;
	if (KEY_PRESSED(buttons.aimbot))
		settings.aimbot = !settings.aimbot;
	if (KEY_PRESSING(VK_LCONTROL))
		if(KEY_PRESSED(0x30))
			settings.fixedCrosshair = !settings.fixedCrosshair;
}


