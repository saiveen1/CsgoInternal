#include "GameData.h"
FLOAT viewProjecitonMatrix[16];


BOOL CEnt::isValidEnt()
{
	if (!this)
		return FALSE;
	if (this == localEnt)
		return FALSE;
	if (this->getHealth() <= 0)
		return FALSE;
	if (this->isDormant())
		return FALSE;
	return TRUE;
}
