#include "Misc.h"
#include "Menu.h"

static VOID FixedCrosshair()
{
	Rect crosshair = { windowWidth / 2.f - 2.f, windowHeight / 2.f - 2.f, 4, 4 };
	render->DrawFilledRect(crosshair, menu->color.crosshair);
}


VOID Misc::run()
{
	if (menu->settings.fixedCrosshair)
		FixedCrosshair();
}


