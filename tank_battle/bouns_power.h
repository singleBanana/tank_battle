#ifndef _BOUNS_POWER_H
#define _BOUNS_POWER_H

#include "bouns_base.h"

#define BOUNS_POWER_VALUE 5

class Bouns_Power :public Bouns_Base
{
public:
	Bouns_Power(float x, float y);

	void OnHit(Tank* tank);
};

#endif