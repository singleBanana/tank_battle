#ifndef _TANK_H
#define _TANK_H

#include "entity.h"
#include "helperfunc.h"
#include "bullet.h"

#include <list>

#define TANK_BULLET_GAP 1.0f
#define TANK_DEF_SPEED 4.0f

class Tank:public Entity
{
	int m_bulletpower;
	int m_teamid;
	float m_speed;

public:
	Tank(float x, float y, float width, float height, int team = 0);
	virtual ~Tank();

	int GetBulletPower();
	void SetBulletPower(int power);

	int GetTeam();

	float GetSpeed();
	void SetSpeed(float sp);

	void SetPosition(b2Vec2& pos); //÷ÿ‘ÿ
	void Fire();

	void Move(ENTITY_DIRECTION dir); //speedŒ™m_speed
	void Move(ENTITY_DIRECTION dir, float speed);
};

#endif