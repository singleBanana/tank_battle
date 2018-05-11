#ifndef _BOUNS_RECOVER_H
#define _BOUNS_RECOVER_H

#include "bouns_base.h"

#define BOUNS_RECOVER_VALUE 10

class Bouns_Recover :public Bouns_Base
{
public:
	Bouns_Recover(float x, float y);

	void OnHit(Tank* tank);
};

#endif