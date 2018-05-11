#ifndef _BLOCK_H
#define _BLOCK_H

#include "entity.h"

class Block :public Entity
{
public:
	Block(float x, float y, float width, float height);
	~Block();

	bool Hit(int attack);
};

#endif