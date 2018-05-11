#include "tank_enemy.h"
#include "helpermacro.h"
#include "resmanager.h"

Tank_Enemy::Tank_Enemy(float x, float y, float width, float height, int team, TANK_ENEMY_TYPE enemytype)
	:Tank(x, y, width, height, team)
{
	ResManager* rm = ResManager::GetInstance();

	switch (enemytype)
	{
	case TANK_ENEMY_TYPE_MOVEFAST:
	{
		m_textureprefix = "enemy_movefast";
		SetTexture(rm->GetTexture(m_textureprefix.c_str()));
		SetMaxHP(40);
		SetHP(40);
		SetSpeed(GetSpeed() * 1.5);
		m_destroypoint = 15;

		break;
	}
	case TANK_ENEMY_TYPE_STRONG:
	{
		m_textureprefix = "enemy_strong";
		SetTexture(rm->GetTexture(m_textureprefix.c_str()));
		SetMaxHP(100);
		SetHP(100);
		SetBulletPower(GetBulletPower() + GetBulletPower() / 2);
		m_destroypoint = 20;

		break;
	}
	case TANK_ENEMY_TYPE_BOSS:
	{
		m_textureprefix = "enemy_boss";
		SetTexture(rm->GetTexture(m_textureprefix.c_str()));
		SetMaxHP(200);
		SetHP(200);
		SetSpeed(GetSpeed() * 1.2);
		SetBulletPower(GetBulletPower() * 2);
		m_destroypoint = 30;

		break;
	}
	default:
	{
		m_textureprefix = "enemy";
		SetTexture(rm->GetTexture(m_textureprefix.c_str()));
		SetMaxHP(50);
		SetHP(50);
		m_destroypoint = 10;
	}
	}
}

bool Tank_Enemy::Hit(int attack)
{
	bool ret = Entity::Hit(attack);

	if (!ret)
	{
		float resthppercent = GetHP() * 1.0 / GetMaxHP();
		string texname(m_textureprefix);
		bool changetex = true;
		if (resthppercent < 0.2 || FLOATEQUAL(resthppercent, 0.2))
		{
			texname += "_20";
		}
		else if (resthppercent < 0.5 || FLOATEQUAL(resthppercent, 0.5))
		{
			texname += "_50";
		}
		else
		{
			changetex = false;
		}

		if (changetex)
		{
			SetTexture(ResManager::GetInstance()->GetTexture(texname.c_str()));
		}
	}

	return ret;
}

int Tank_Enemy::GetDestroyPoint()
{
	return m_destroypoint;
}