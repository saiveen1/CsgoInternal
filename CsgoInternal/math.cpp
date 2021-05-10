#include "math.h"
#include "GameData.h"


BOOL Math::WorldToScreen(Vec3 pos, Vec2* screen) {

	Vec4 clipCoords;
	clipCoords.x = pos.x * viewProjecitonMatrix[0] + pos.y * viewProjecitonMatrix[1] + pos.z * viewProjecitonMatrix[2] + viewProjecitonMatrix[3];
	clipCoords.y = pos.x * viewProjecitonMatrix[4] + pos.y * viewProjecitonMatrix[5] + pos.z * viewProjecitonMatrix[6] + viewProjecitonMatrix[7];
	clipCoords.z = pos.x * viewProjecitonMatrix[8] + pos.y * viewProjecitonMatrix[9] + pos.z * viewProjecitonMatrix[10] + viewProjecitonMatrix[11];
	clipCoords.w = pos.x * viewProjecitonMatrix[12] + pos.y * viewProjecitonMatrix[13] + pos.z * viewProjecitonMatrix[14] + viewProjecitonMatrix[15];

	if (clipCoords.w < 0)
		return FALSE;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen->x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen->y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	//Ðè¸Ä½ø...
	if (screen->x > windowWidth * 6.f || screen->y > windowHeight * 6.f || screen->x < 0 || screen->y < 0)
		return FALSE;
	return TRUE;
}

FLOAT Math::GetDistance2D(Vec2 src, Vec2 dst)
{
	return sqrtf((src.x - dst.x) * (src.x - dst.x) + (src.y - dst.y) * (src.y - dst.y));
}
