#include "Misc.h"
#include "Menu.h"
#include "GameData.h"
static VOID DrawRecoCrosshair(Vec2 dst, FLOAT crosshairSize)
{
	Vec2 leftSrc, rightDst, upSrc, downDst;
	leftSrc.x = dst.x - crosshairSize;
	rightDst.x = dst.x + crosshairSize;
	leftSrc.y = rightDst.y = dst.y;

	upSrc.y = dst.y - crosshairSize;
	downDst.y = dst.y + crosshairSize;
	upSrc.x = downDst.x = dst.x;
	auto lineLength = crosshairSize - 6.f;

	Rect crosshair = { dst.x, dst.y, 1, 1 };
	//render->DrawFilledRect(crosshair, menu->color.crosshair);
	render->DrawLine(dst, dst + 1.f, 1, menu->color.crosshair);
	render->DrawLine(leftSrc, {leftSrc.x+ lineLength,leftSrc.y}, 1, menu->color.crosshair);
	render->DrawLine(rightDst, { rightDst.x- lineLength,rightDst.y}, 1, menu->color.crosshair);
	render->DrawLine(upSrc, { upSrc.x,upSrc.y+lineLength }, 1, menu->color.crosshair);
	render->DrawLine(downDst, { downDst.x,downDst.y-lineLength }, 1, menu->color.crosshair);
}

static VOID RecoilCrosshair()
{
	if (g_localEnt->isScoped())
		return;
	Vec2 crosshairPos2D = GameData::GetRecoilCrosshair2D();


	DrawRecoCrosshair(crosshairPos2D, menu->settings.crosshairSize);

	//¹Ì¶¨×¼ÐÇ
	//Rect crosshair = { windowWidth / 2.f - 2.f, windowHeight / 2.f - 2.f, 4, 4 };
	//render->DrawFilledRect(crosshair, menu->color.crosshair);
}


VOID Misc::run()
{
	if (menu->settings.recoilCrosshair)
		RecoilCrosshair();
}


