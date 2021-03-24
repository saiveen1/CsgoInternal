#include "Aimbot.h"
#include <atlstr.h>

#include "hack.h"
#include "Menu.h"
#include "utils.h"
#include "render.h"
struct BestAngle {
	//最近ent和ent骨骼的距离
	CEnt* bestAimEnt = NULL;
	FLOAT closestBoneDist = 10000.f;
	Vec3 bestBonePos3D = { 0 };
	UINT count = 0;
	VOID update() {
		bestAimEnt = NULL;
		closestBoneDist = 10000.f;
		bestBonePos3D = { 0 };
	}
}bestAngle;

struct Settings {
	FLOAT aimRange = 88.f;
	FLOAT aimSpeed = 3.f;
	UINT tickCount = 0;
	VOID operator++() {
		tickCount++;
		if (tickCount == 66)
			tickCount = 0;
	}

	VOID update() {
		menu->settings.showAimbotRange = FALSE;
		if (KEY_PRESSING(VK_LMENU))
		{
			menu->settings.showAimbotRange = TRUE;
			if (KEY_PRESSED(VK_UP))
				aimRange += 2.f;
			if (KEY_PRESSED(VK_DOWN))
				aimRange -= 2.f;
			if (KEY_PRESSED(VK_LEFT))
				aimSpeed += 0.2f;
			if (KEY_PRESSED(VK_RIGHT))
				aimSpeed -= 0.2f;
			if (aimSpeed < 1)
				aimSpeed = 1;
		}
	}
}aimbotSettings;

Vec3 oldPunch = { 0 };
static VOID NoRecoil() 
{
	auto aimPunch = g_localEnt->getAimPunchAngle();
	if (g_localEnt->getShotsFired() > 1)
	{
		Vec3 hitPoint = *clientData.viewAngles - aimPunch * 2 + oldPunch;
		GameData::SetLocalViewAnlges(hitPoint);
	}
	oldPunch = aimPunch * 2;
}

static VOID CalcBestAngles()
{
	bestAngle.update();
	for (auto player : hack->players)
	{
		if(player->getTeam() == g_localEnt->getTeam())
			continue;
		//简单的是否可见判断
		if(!player->getSpottedMask())
			continue;
		auto i = 0;
		for (auto bonePoint : GameData::GetBonePoints(player))
		{
			//只瞄身子, 不加判断就是对所有骨骼点进行判定(骨骼图)
			if (i++ < 7)
			{
				auto bonePos3D = GameData::GetBonePos3D(player, bonePoint);
				auto bonePos2D = Vec2{ 0 };
				if (W2S(bonePos3D, &bonePos2D))
				{
					//可以改成根据后坐力准星计算
					auto curBoneDist = GameData::GetDistFromCrosshair(bonePos2D);
					//超过自瞄范围
					if(curBoneDist > aimbotSettings.aimRange)
						continue;
					if (curBoneDist < bestAngle.closestBoneDist)
					{
						bestAngle.closestBoneDist = curBoneDist;
						bestAngle.bestBonePos3D = bonePos3D;
						bestAngle.bestAimEnt = player;
					}
				}
			}
		}
	}
}

VOID Aimbot::run()
{
	aimbotSettings.update();
	if (menu->settings.noRecoil)
		NoRecoil();
	if (menu->settings.showAimbotRange)
	{
		CString szTemp;
		szTemp.Format(L"%.1f", aimbotSettings.aimSpeed);
		render->DrawTextW(szTemp, SCREENCENTER, WHITE);
		render->DrawCircle(SCREENCENTER, aimbotSettings.aimRange, (UINT)(aimbotSettings.aimRange / 3), 1, WHITE);
	}
	if (menu->settings.aimbot)
	{
		menu->settings.noRecoil = FALSE;
		CalcBestAngles();
		if (KEY_PRESSING(menu->buttons.startAim))
		{
			//不能用&&判断
			if (bestAngle.bestAimEnt) 
			{
				auto newViewAngles = GameData::GetAimAnlges(bestAngle.bestBonePos3D);
				auto aimPunch = g_localEnt->getAimPunchAngle();
				newViewAngles -= aimPunch * 2;	//只需要打一个点所以不用修正
				auto curViewAngles = g_localEnt->getViewAngles();
				auto delta = (newViewAngles - curViewAngles).Normalize();
				GameData::SetLocalViewAnlges(curViewAngles + delta / aimbotSettings.aimSpeed);
			}
		}
	}
}
