#ifndef _BOX2DDEF_H
#define _BOX2DDEF_H

#include <Box2D\Box2D.h>

#define BOX2D_SCALE_RATIO 32.0f //BOX2D_SCALE_RATIO¸öÏñËØ=1Ã×

struct Vec2b2fixture
{
	b2Fixture* f1;
	b2Fixture* f2;

	bool operator ==(const Vec2b2fixture& other) const
	{
		return ((f1 == other.f1) && (f2 == other.f2));
	}
};

#endif