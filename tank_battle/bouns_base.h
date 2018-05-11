#ifndef _BOUNS_BASE_H
#define _BOUNS_BASE_H

#include "entity.h"
#include "tank.h"

#define BOUNS_WIDTH 32
#define BOUNS_HEIGHT 32

class Bouns_Base :public Entity
{
public:
	Bouns_Base(float x, float y);
	virtual ~Bouns_Base();

	virtual void OnHit(Tank* tank) = 0;
};

#endif