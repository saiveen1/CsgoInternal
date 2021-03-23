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
};

extern FLOAT viewProjecitonMatrix[16];
extern EntData playerData;
extern CEnt* localEnt;
//还有个clientData在后面

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
	CEnt* pLocal;
};
extern ClientData clientData;

class CEnt {

private:
public:

	BOOL isValidEnt();

	inline BYTE isDormant();
	inline INT getTeam();
	inline INT getHealth();
	inline INT getFlags();
	inline FLOAT getEyePosDelta();
	inline Vec3 getViewAngles();
	inline Vec3 getAimPunchAngles();
	inline Vec3 getVecOrigin();
	inline INT getBoneMatrix();
	inline INT getBonePointInfo();
};

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
Vec3 CEnt::getAimPunchAngles() {
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



