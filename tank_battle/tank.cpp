#include "tank.h"
#include "gameplay.h"
#include "helperfunc.h"
#include "helpermacro.h"
#include "constdef.h"
#include "particlemanager.h"
#include "resmanager.h"

Tank::Tank(float x, float y, float width, float height, int team)
	:Entity(x, y, width, height, 0, false, false, false, std::string("tank"))
{
	SetHP(100);

	m_bulletpower = BULLET_ATTACK;
	m_teamid = team;
	m_speed = TANK_DEF_SPEED;

	ParticleManager::GetInstance()->SpawnParticle("tank_spawn", GetPositon().x, GetPositon().y);
}

Tank::~Tank()
{
	ParticleManager::GetInstance()->SpawnParticle("tank_destroy", GetPositon().x, GetPositon().y);
}

int Tank::GetBulletPower()
{
	return m_bulletpower;
}

void Tank::SetBulletPower(int power)
{
	m_bulletpower = power;
}

int Tank::GetTeam()
{
	return m_teamid;
}

float Tank::GetSpeed()
{
	return m_speed;
}

void Tank::SetSpeed(float sp)
{
	m_speed = sp;
}

void Tank::SetPosition(b2Vec2& pos)
{
	SetLinerVelocity(b2Vec2(0, 0));
	Entity::SetPosition(pos);
}

void Tank::Fire()
{
	b2Vec2 tankpos = GetPositon();
	b2Vec2 tanksize = GetSize();
	float tankrot = GetRotation();

	int bulletwidth = 8;
	int bulletheight = 8;
	float bulletx, bullety;

	switch (GetEntityDirection(tankrot))
	{
	case ENTITY_DIRECTION_UP:
	{
		bulletx = tankpos.x - bulletwidth / 2;
		bullety = tankpos.y - bulletheight - tanksize.y / 2 - TANK_BULLET_GAP;

		break;
	}
	case ENTITY_DIRECTION_DOWN:
	{
		bulletx = tankpos.x - bulletwidth / 2;
		bullety = tankpos.y + tanksize.y / 2 + TANK_BULLET_GAP;

		break;
	}
	case ENTITY_DIRECTION_LEFT:
	{
		bulletx = tankpos.x - tanksize.x / 2 - bulletwidth - TANK_BULLET_GAP;
		bullety = tankpos.y - bulletheight / 2;

		break;
	}
	case ENTITY_DIRECTION_RIGHT:
	{
		bulletx = tankpos.x + tanksize.x / 2 + TANK_BULLET_GAP;
		bullety = tankpos.y - bulletheight / 2;

		break;
	}
	}

	Bullet* bullet = new Bullet(bulletx, bullety, bulletwidth, bulletheight, tankrot, this);
	bullet->SetAttack(m_bulletpower);
	bullet->SetTexture(ResManager::GetInstance()->GetTexture("bullet"));
}

void Tank::Move(ENTITY_DIRECTION dir)
{
	Move(dir, m_speed);
}

void Tank::Move(ENTITY_DIRECTION dir, float speed)
{
	b2Vec2 pos = GetPositon();
	float rot = GetRotation();
	switch (dir)
	{
	case ENTITY_DIRECTION_UP:
	{
		pos.y -= speed;
		rot = 0;
		break;
	}
	case ENTITY_DIRECTION_DOWN:
	{
		pos.y += speed;
		rot = MATH_PI;
		break;
	}
	case ENTITY_DIRECTION_LEFT:
	{
		pos.x -= speed;
		rot = MATH_PI * 3 / 2;
		break;
	}
	case ENTITY_DIRECTION_RIGHT:
	{
		pos.x += speed;
		rot = MATH_PI / 2;
		break;
	}
	}

	SetPosition(pos);
	SetRotation(rot);
}