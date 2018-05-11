#ifndef _BOUNS_H
#define _BOUNS_H

#include "bouns_base.h"

#define BOUNS_Z 0.2

enum BOUNS_TYPE
{
	BOUNS_TYPE_RECOVER,
	BOUNS_TYPE_POWER,
	BOUNS_TYPE_PLUSPOINT,
	BOUNS_TYPE_INVINCIBLE,
	BOUNS_TYPE_SPEEDUP
};

Bouns_Base* RandBouns();

#endif