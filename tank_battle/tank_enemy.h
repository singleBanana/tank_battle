#ifndef _TANK_ENEMY_H
#define _TANK_ENEMY_H

#include "tank.h"

enum TANK_ENEMY_TYPE
{
	TANK_ENEMY_TYPE_NORMAL = 0,
	TANK_ENEMY_TYPE_MOVEFAST,
	TANK_ENEMY_TYPE_STRONG,
	TANK_ENEMY_TYPE_BOSS
};

class Tank_Enemy:public Tank
{
	string m_textureprefix;
	int m_destroypoint;

public:
	Tank_Enemy(float x, float y, float width, float height, int team = 0, TANK_ENEMY_TYPE enemytype = TANK_ENEMY_TYPE_NORMAL);

	bool Hit(int attack);

	int GetDestroyPoint();
};

#endif