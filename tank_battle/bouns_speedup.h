#ifndef _BOUNS_SPEEDUP_H
#define _BOUNS_SPEEDUP_H

#include "bouns_base.h"

#define BOUNS_SPEEDUP_TIME 5
#define BOUNS_SPEEDUP_VALUE 2

class Bouns_SpeedUp :public Bouns_Base
{
	struct BounsSpeedUpData
	{
		Tank* tank;
		int prespeed;
	};

public:
	Bouns_SpeedUp(float x, float y);

	void OnHit(Tank* tank);

	void SpeedUpEnd(void* data, bool isoncleancall);
};

#endif