
#include "esp.h"
#include "render.h"
#include "hack.h"
#include "Menu.h"
#include "math.h"
#include "utils.h"
#pragma warning(disable:26812) //enum class warning
D3DCOLOR espColor, snaplineColor, skeletonColor;

//�����ã��ֶ�����
#ifdef _DEBUG

#include<atlstr.h>//wsprintf ֧�ָ�������ʽ��

int headBone = 9;
Vec3 drawLinePos = { 0 };
Vec3 originalPos = { 0 };
int dbgCount = 0;
WCHAR szBufffer[256]{ 0 };
CString str;
struct DbgSets {
	//��ʾ���й�����
	//Show all bone points
	BOOL bAllBones = FALSE; 
	//��ʾ��Ҫ������
	//Show main bone(hitBox) points
	BOOL bMainBonePoints = FALSE; 
	//��������82�ƶ�һ���� 9����Ϊ����ԭ�� ��ֻ��һ��ʵ���ʱ��ʹ�ã�������
	//press arrow keys and NUMPAD8_2 to move the line, NUMPAD9 to reset the line to ent
	//Use when there is only one ent
	BOOL bMovingLine = FALSE; 
} dbgSettins;
D3DCOLOR textColor = D3DCOLOR_ARGB(255, 255, 255, 255);

static VOID renderAllBones(CEnt* ent)
{
	UINT_PTR addr = *(UINT*)((UINT_PTR)ent + playerData.boneMatrix);
	UINT_PTR fatherBoneAddr = 0;
	UINT_PTR childBoneAddr = 0;
	Vec3 fatherBonePos3D = { 0 };
	Vec3 cBonePos3D = { 0 };
	Vec3* pFatherBonePos3D = new Vec3{ 0 };
	Vec3* pChildBonePos3D = new Vec3{ 0 };
	for (auto k = 0; k < 0x5c; k++) {
		fatherBoneAddr = addr + 0x30 * k;
		childBoneAddr = fatherBoneAddr + 0x30;
		for (auto j = 0; j < 3; j++) {
			*((FLOAT*)pFatherBonePos3D + j) = *(FLOAT*)(fatherBoneAddr + 0x10 * j + 0xc);

			*((FLOAT*)pChildBonePos3D + j) = *(FLOAT*)(childBoneAddr + 0x10 * j + 0xc);
		}

		Vec2 fBonePos2D, cBonePos2D;
		W2S(*pFatherBonePos3D, &fBonePos2D);
		W2S(*pChildBonePos3D, &cBonePos2D);
		//DrawLine(fBonePos2D.x, fBonePos2D.y, cBonePos2D.x, cBonePos2D.y, 1, snaplineColor);
		str.Format(L"%d", k);
		memset(szBufffer, 0, 256 * 2);
		wsprintf(szBufffer, L"%s", str.GetBuffer(str.GetLength()));
		render->DrawText(szBufffer, fBonePos2D, textColor);
	}
	delete pFatherBonePos3D;
	delete pChildBonePos3D;
}	//��ʾ���й�����

static VOID movingLine(CEnt* ent, Vec2 basePos2D)//�������Һ�С����8 2һ���ƶ����ߣ���ֻ��һ��ʵ���ʱ��ʹ�ã�������
{
	if (!dbgCount || KEY_PRESSED(VK_NUMPAD8))
		drawLinePos = originalPos = ent->getVecOrigin();
	//��������
	if (KEY_PRESSED(VK_NUMPAD9))
	{
		Vec3 delta = originalPos - drawLinePos;
		drawLinePos = ent->getVecOrigin();
		drawLinePos = drawLinePos - originalPos;
	}
	if (KEY_PRESSED(VK_UP))
		drawLinePos.x++;
	if (KEY_PRESSED(VK_DOWN))
		drawLinePos.x--;
	if (KEY_PRESSED(VK_LEFT))
		drawLinePos.y++;
	if (KEY_PRESSED(VK_RIGHT))
		drawLinePos.y--;
	if (KEY_PRESSED(VK_NUMPAD8))
		drawLinePos.z++;
	if (KEY_PRESSED(VK_NUMPAD2))
		drawLinePos.z--;

	str.Format(L"%f %f %f", drawLinePos.x, drawLinePos.y, drawLinePos.z);
	wsprintf(szBufffer, L"LineSourceWorldPos: %s", str.GetBuffer(str.GetLength()));
	Vec2 showPos = { windowWidth / 2.f,60 };
	render->DrawText(szBufffer, showPos, textColor);

	if (W2S(drawLinePos, &basePos2D))
		render->DrawLine(basePos2D.x, basePos2D.y, (FLOAT)(windowWidth / 2), (FLOAT)windowHeight, 2, snaplineColor);
}

#endif // _DEBUG


static VOID DrawSnapLine(Vec2 pos2D)
{
	render->DrawLine(pos2D, Vec2{ windowWidth / 2.f, (FLOAT)windowHeight }, 1, snaplineColor);
}

static VOID DrawEspBox2D(Vec2 top, Vec2 bottom, FLOAT thickness, D3DCOLOR color)
{
	auto height = abs(top.y - bottom.y);
	Vec2 topLeft;

	topLeft.x = top.x - height / 4;
	topLeft.y = top.y;

	Rect rect = { topLeft.x,topLeft.y, height / 2, height };
	render->DrawRect(rect, thickness, color);
}

static VOID TopFixedHealthBar(CEnt* ent,Vec2 headPos2D)
{
	FLOAT width = 40.f;
	FLOAT height = 15.f;

	FLOAT topLX = headPos2D.x - width / 2;
	FLOAT topLY = headPos2D.y - height;
	//�߶ȼ�2�����ز���ס͸�ӿ�
	Rect rect = { topLX, topLY, width, height };

	FLOAT healthPerc = ent->getHealth() / 100.f;
	FLOAT updateWidth = width * healthPerc;
	Rect filledRect = rect;
	filledRect.width = updateWidth;
	filledRect += 2; //��2�����ز����ǿ�
	render->DrawRectWithFilledCol(rect, filledRect, 0.6f, menu->color.enemy.healthRect, menu->color.healthBar);
}

static VOID TopDymaicHealthBar(CEnt* ent, Vec2 basePos2D,Vec2 headPos2D)
{
	FLOAT height = abs(headPos2D.y - basePos2D.y);
	FLOAT width = height / 2.f;
	height = width / (FLOAT)2.5;
	FLOAT topLX = headPos2D.x - width / 2.f;
	FLOAT topLY = headPos2D.y - height / 2.f;

	Rect rect = { topLX, topLY, width, height / 2.f };

	FLOAT healthPerc = ent->getHealth() / 100.f;
	FLOAT updateWidth = width * healthPerc;
	Rect filledRect = rect;
	filledRect.width = updateWidth;
	filledRect += 1; //��1�����ز����ǿ�
	render->DrawRectWithFilledCol(rect, filledRect, 1.f, BLACK, menu->color.healthBar);
}

static VOID LeftHealthBar(CEnt* ent, Vec2 basePos2D, Vec2 headPos2D)
{
	auto rectHeight = abs(headPos2D.y - basePos2D.y);
	auto rectWidth = rectHeight / 2.f;
	auto healthBarWidth = 3.f;
	auto topLX = headPos2D.x - rectWidth / 2.f - healthBarWidth;
	auto topLY = headPos2D.y + 2.f; //�����Ļ�Ѫ������������һ��
	Rect filledRect = { topLX,topLY,healthBarWidth,rectHeight };
	auto healthPerc = ent->getHealth() / 100.f;
	auto updateHeight = filledRect.height * healthPerc;
	filledRect.topLeftY += (filledRect.height - updateHeight);
	filledRect.height = updateHeight;
	render->DrawFilledRect(filledRect, menu->color.healthBar);
}

static VOID RenderHealthBar(UINT hpStyle,CEnt* ent, Vec2 basePos2D,Vec2 headPos2D)
{
	switch (hpStyle)
	{
	case topFixed:
	{
		TopFixedHealthBar(ent, headPos2D);
		break;
	}
	case topBoxWithRect:
	{
		TopDymaicHealthBar(ent, basePos2D, headPos2D);
		break;
	}
	case lineLeft:
	{
		//ֻ�п��˷�������Ѫ��������
		if (menu->settings.visual.rect)
			LeftHealthBar(ent, basePos2D, headPos2D);
		break;
	}
	default:
		break;
	}
}

static VOID RenderSkeleton(CEnt* ent)
{
	//����Ҫ������(�����ж�)
	auto bonePoints = GameData::GetBonePoints(ent);

	UINT_PTR addr = *(UINT*)((UINT_PTR)ent + playerData.boneMatrix);
	UINT_PTR fatherBoneAddr = 0;
	UINT_PTR childBoneAddr = 0;
	Vec3 fatherBonePos3D = { 0 };
	Vec3 cBonePos3D = { 0 };

	for (auto j = 1ul; j < bonePoints.size(); j++)
	{
		//�����±���0�Ź�����, ��3�Ź��������һ��
		if (j != 2)
		{
			auto fatherBonePoint = bonePoints.at(j);
			fatherBonePos3D = GameData::GetBonePos3D(ent, fatherBonePoint);
			cBonePos3D = GameData::GetBonePos3D(ent, fatherBonePoint + 1);

			fatherBoneAddr = addr + 0x30 * bonePoints.at(j);
			childBoneAddr = fatherBoneAddr + 0x30;


			Vec2 fBonePos2D, cBonePos2D;
			if (W2S(fatherBonePos3D, &fBonePos2D) && W2S(cBonePos3D, &cBonePos2D))
			{
#ifdef _DEBUG
				if (dbgSettins.bMainBonePoints)//��ʾ��Ҫ���ж�������
				{
					str.Format(L"%d", bonePoints.at(j));
					memset(szBufffer, 0, 256 * 2);
					wsprintf(szBufffer, L"%s", str.GetBuffer(str.GetLength()));
					render->DrawText(szBufffer, fBonePos2D, textColor);
				}
#endif // _DEBUG

				//�ֲ�����
				if (j == 13 || j == 14)
					continue;
				render->DrawLine(fBonePos2D, cBonePos2D, 1, skeletonColor);

				//���Ӻʹ������Ӵ�
				if (j == 1 || j == 3)
				{
					DWORD childBones[2];

					Vec3 connects3D[2];
					Vec2 connects2D[2];
					childBones[0] = j == 1 ? bonePoints[15] : bonePoints[7];
					childBones[1] = j == 1 ? bonePoints[17] : bonePoints[8];
					for (auto k = 0; k < 2; k++)
					{
						connects3D[k] = GameData::GetBonePos3D(ent, childBones[k]);
						if (W2S(connects3D[k], &connects2D[k]))
							render->DrawLine(fBonePos2D, connects2D[k], 1, skeletonColor);
					}
					//ͷ����ԲȦ
					if (j == 1)
						render->DrawCircle(cBonePos2D, Math::GetDistance2D(fBonePos2D, cBonePos2D), 20, 1, skeletonColor);
				}
			}
		}
	}
}

static VOID RenderPlayerEsp(CEnt* ent)
{
	Vec2 basePos2D = { 0 };
	if (W2S(ent->getVecOrigin(), &basePos2D))
	{
		Vec3 headPos3D = { 0 };
		Vec2 headPos2D = { 0 };
		headPos3D = GameData::GetHeadPos(ent);

		W2S(headPos3D, &headPos2D);
		if (menu->settings.visual.snaplines)
			DrawSnapLine(basePos2D);
		if (menu->settings.visual.rect)
			DrawEspBox2D(headPos2D, basePos2D, 1, espColor);
		if (menu->settings.visual.healthBar)
			RenderHealthBar(menu->settings.visual.healthBar, ent, basePos2D, headPos2D);
		if (menu->settings.visual.skeletonEsp)
			RenderSkeleton(ent);
#ifdef _DEBUG
		if (dbgSettins.bAllBones)
			renderAllBones(ent);
		if (dbgSettins.bMovingLine)
			movingLine(ent, basePos2D);
#endif // _DEBUG

	}
}

VOID ESP::run()
{
	for (auto player : hack->players) 
	{
		if (player->getTeam() == g_localEnt->getTeam())
		{
			if (!menu->settings.visual.showTeamates)
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
