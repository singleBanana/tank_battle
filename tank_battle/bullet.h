#ifndef _BULLET_H
#define _BULLET_H

#include "entity.h"

#define BULLET_SPEED 20
#define BULLET_ATTACK 10

class Tank;

class Bullet:public Entity
{
	Tank* m_owner;

	int m_attack;

public:
	Bullet(float x, float y, float width, float height, float rot, Tank* owner);
	~Bullet();

	Tank* GetOwner();
	int GetAttack();
	void SetAttack(int attack);
};

#endif