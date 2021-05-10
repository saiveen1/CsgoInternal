#pragma once
#include <windows.h>
#include "math.h"


struct EntData {
	UINT dormant;
	UINT teamNum;
	UINT health;
	UINT flags;
	UINT eyePosDelta;
	UINT viewAngles;
	UINT aimPunchAngles;
	UINT vecOrigin;
	UINT boneMatrix;
	UINT bonePointInfo;
	UINT shitBonePointOffset;
	UINT spottedMask;
	UINT shotsFired;
	UINT scoped;
};

extern FLOAT viewProjecitonMatrix[16];
extern EntData playerData;

//还有localEnt和clientData在后面

class CEntInfo
{
public:

	class CEnt* pEnt; //0x0000
	int32_t serialNum; //0x0004
	class CEntInfo* pPrev; //0x0008
	class CEntInfo* pNext; //0x000C
}; //Size: 0x0010

class CEntInfoList
{
public:
	class CEntInfo* m_pHead; //0x0000
	class CEntInfo* m_pTail; //0x0004
}; //Size: 0x0008

class CBaseEntityList
{
public:
	class CEntInfo m_EntPtrArrary[8192]; //0x0000
	class CEntInfoList m_activeList; //0x20000
	class CEntInfoList m_freeNonNetworkableList; //0x20008
}; //Size: 0x20010


struct ClientData {
	CBaseEntityList* entityList;
	FLOAT* vpMatrix;
	UINT* clientState;
	Vec3* viewAngles;
	CEnt** pLocalPtr;
};
extern ClientData clientData;

class CEnt {

private:
public:

	inline BYTE isDormant();
	inline INT getTeam();
	inline INT getHealth();
	inline INT getFlags();
	inline FLOAT getEyePosDelta();
	inline Vec3 getViewAngles();
	inline Vec3 getAimPunchAngle();
	inline Vec3 getVecOrigin();
	inline INT getBoneMatrix();
	inline INT getBonePointInfo();
	inline BOOL getSpottedMask();
	inline INT getShotsFired();
	inline BOOL isScoped();
};
extern CEnt* g_localEnt;

BYTE CEnt::isDormant() {
	return *(BYTE*)((UINT_PTR)this + playerData.dormant);
}
INT CEnt::getTeam() {
	return *(INT*)((UINT_PTR)this + playerData.teamNum);
}
INT CEnt::getHealth() {
	return *(INT*)((UINT_PTR)this + playerData.health);
}
INT CEnt::getFlags() {
	return *(INT*)((UINT_PTR)this + playerData.flags);
}
FLOAT CEnt::getEyePosDelta() {
	return *(FLOAT*)((UINT_PTR)this + playerData.eyePosDelta);
}
Vec3 CEnt::getViewAngles() {
	return *(Vec3*)((UINT_PTR)this + playerData.viewAngles);
}
Vec3 CEnt::getAimPunchAngle() {
	return *(Vec3*)((UINT_PTR)this + playerData.aimPunchAngles);
}
Vec3 CEnt::getVecOrigin() {
	return *(Vec3*)((UINT_PTR)this + playerData.vecOrigin);
}
INT CEnt::getBoneMatrix() {
	return *(INT*)((UINT_PTR)this + playerData.boneMatrix);
}
INT CEnt::getBonePointInfo() {
	return *(INT*)((UINT_PTR)this + playerData.bonePointInfo);
}
BOOL CEnt::getSpottedMask() {
	return *(INT*)((UINT_PTR)this + playerData.spottedMask);
}
INT CEnt::getShotsFired() {
	return *(INT*)((UINT_PTR)this + playerData.shotsFired);
}
BOOL CEnt::isScoped() {
	return *(BOOL*)((UINT_PTR)this + playerData.scoped);
}

namespace GameData 
{
	BOOL isValidEnt(CEnt* ent);
	Vec3 GetBonePos3D(CEnt* ent, UINT bone);
	Vec3 GetHeadPos(CEnt* ent);
	std::vector<UINT> GetBonePoints(CEnt* ent);
	Vec3 GetLocalViewAngles();
	VOID SetLocalViewAnlges(Vec3 newAngles);
	Vec3 GetAimAnlges(Vec3 src);
	FLOAT GetDistFromCrosshair(Vec2 src);
	Vec2 GetRecoilCrosshair2D();
}

