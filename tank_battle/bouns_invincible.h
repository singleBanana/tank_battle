#ifndef _BOUNS_INVINCIBLE_H
#define _BOUNS_INVINCIBLE_H

#include "bouns_base.h"

#define BOUNS_INVINCIBLE_TIME 5

class Bouns_Invincible :public Bouns_Base
{
public:
	Bouns_Invincible(float x, float y);

	void OnHit(Tank* tank);

	void InvincibleEnd(void* tank, bool isoncleancall);
};

#endif