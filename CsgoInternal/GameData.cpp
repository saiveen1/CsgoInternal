#include <math.h>
#include <cmath>

#include "GameData.h"

FLOAT viewProjecitonMatrix[16];


BOOL GameData::isValidEnt(CEnt* ent)
{
	if (!ent)
		return FALSE;
	if (ent == g_localEnt)
		return FALSE;
	if (ent->getHealth() <= 0)
		return FALSE;
	if (ent->isDormant())
		return FALSE;
	return TRUE;
}

Vec3 GameData::GetBonePos3D(CEnt* ent, UINT bone)
{
	uintptr_t bonePtr = ent->getBoneMatrix();
	Vec3 bonePos;
	bonePos.x = *(FLOAT*)(bonePtr + 0x30 * bone + 0x0C);
	bonePos.y = *(FLOAT*)(bonePtr + 0x30 * bone + 0x1C);
	bonePos.z = *(FLOAT*)(bonePtr + 0x30 * bone + 0x2C);

	return bonePos;
}

Vec3 GameData::GetHeadPos(CEnt* ent)
{
	//这里得到的是viewPos, 骨骼图中的眼位置, 所以画方框之类的要再加半个头的大小.
	Vec3 viewPos = GetBonePos3D(ent, 8);
	viewPos.z += 8;
	return viewPos;
}

std::vector<UINT> GameData::GetBonePoints(CEnt* ent)
{

	UINT_PTR bonePointAddr = 0;
	// [294c] + [[294c]+b0] + [[294c] + [[294c]+b0]+8] + 0x44*i
	CONST UINT_PTR reBPR = *(UINT*)((UINT_PTR)ent + playerData.bonePointInfo);
	CONST UINT_PTR reBPR_R = *(UINT*)(reBPR);
	CONST UINT reBPR_R_B0R = *(UINT*)(reBPR_R + playerData.shitBonePointOffset);
	CONST UINT_PTR reBPR_b0R_reBPR_8R = *(UINT*)(reBPR_R + reBPR_R_B0R + 8);

	auto bonePoints = std::vector<UINT>{};
	for (auto j = 0ul;; j++)
	{
		bonePointAddr = reBPR_R + reBPR_R_B0R + reBPR_b0R_reBPR_8R + 0x44 * j;
		UINT bPoint = *(UINT*)bonePointAddr;
		if (j != 0)
			if (bPoint > 0x60) //后面的值明显不是骨骼点
				break;
		bonePoints.push_back(bPoint);
	}
	return bonePoints;
}

Vec3 GameData::GetLocalViewAngles()
{
	return *(clientData.viewAngles);
}

VOID GameData::SetLocalViewAnlges(Vec3 newAngles)
{
	newAngles.Normalize();
	clientData.viewAngles->x = newAngles.x;
	clientData.viewAngles->y = newAngles.y;
	clientData.viewAngles->z = newAngles.z;
}

Vec3 GameData::GetAimAnlges(Vec3 src)
{
	//摄影机位置并不是在骨骼点8的位置
	auto eyePos = GetBonePos3D(g_localEnt, 2);
	auto delta = src - eyePos;

	auto pitch = ANGLE(-std::atanf(delta.z / hypot(delta.x, delta.y)));
	auto yaw = ANGLE(atan2(delta.y, delta.x));
	Vec3 angles = { pitch,yaw,0 };

	return angles;
}

FLOAT GameData::GetDistFromCrosshair(Vec2 src)
{
	return Math::GetDistance2D(src, { windowWidth / 2.f,windowHeight / 2.f });
}

