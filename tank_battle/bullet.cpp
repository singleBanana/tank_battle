#include "bullet.h"
#include "tank.h"
#include "helperfunc.h"

Bullet::Bullet(float x, float y, float width, float height, float rot, Tank* owner)
	:Entity(x, y, width, height, rot, false, false, true, std::string("bullet"))
{
	m_owner = owner;
	m_attack = BULLET_ATTACK;

	SetLinerVelocity(GenVecFromDirection(GetEntityDirection(rot), BULLET_SPEED));
}

Bullet::~Bullet()
{

}

Tank* Bullet::GetOwner()
{
	return m_owner;
}

int Bullet::GetAttack()
{
	return m_attack;
}

void Bullet::SetAttack(int attack)
{
	m_attack = attack;
}