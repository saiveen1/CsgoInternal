
#include "esp.h"
#include "render.h"
#include "hack.h"
#include "Menu.h"
static VOID DrawEspBox2D(Vec2 top, Vec2 bottom, FLOAT thickness, D3DCOLOR color) 
{
	auto height = abs(top.y - bottom.y);
	Vec2 topLeft;

	topLeft.x = top.x - height / 4;
	topLeft.y = top.y;

	Rect rect = { topLeft.x,topLeft.y, height / 2, height };
	render->DrawRect(rect, thickness, color);
}

static VOID RenderPlayerEsp(CEnt* ent)
{

}

D3DCOLOR espColor, snaplineColor, skeletonColor;
VOID ESP::run()
{
	for (auto player : hack->players) 
	{
		if (player->isValidEnt())
		{
			if (player->getTeam() == localEnt->getTeam())
			{
				if (menu->settings.showTeamates)
					continue;
				espColor = menu->color.team.esp;
				snaplineColor = menu->color.team.snapline;
				skeletonColor = menu->color.team.skeleton;
			}
			else
			{
				espColor = menu->color.enemy.esp;
				snaplineColor = menu->color.enemy.snapline;
				skeletonColor = menu->color.enemy.skeleton;
			}
			RenderPlayerEsp(player);
		}
	}
}
