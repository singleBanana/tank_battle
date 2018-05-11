#ifndef _BOUNS_PLUSPOINT_H
#define _BOUNS_PLUSPOINT_H

#include "bouns_base.h"

class Bouns_PlusPoint :public Bouns_Base
{
public:
	Bouns_PlusPoint(float x, float y);

	void OnHit(Tank* tank);
};

#endif