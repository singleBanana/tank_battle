#include "ai_player.h"
#include "helpermacro.h"
#include "audiomanager.h"
#include "resmanager.h"

AI_Player::AI_Player(Tank* tank) : AI_Base(tank)
{
	m_firecd = new Second_Cycle(AI_PLAYER_CD_FIRE);

	m_efffire = ResManager::GetInstance()->GetEffect("tank_fire");
}

AI_Player::~AI_Player()
{
	SAFE_DELETE(m_firecd);
}

void AI_Player::Think()
{
	if (KEYSTATE(HGEK_W))
	{
		m_tank->Move(ENTITY_DIRECTION_UP);
	}
	if (KEYSTATE(HGEK_S))
	{
		m_tank->Move(ENTITY_DIRECTION_DOWN);
	}
	if (KEYSTATE(HGEK_A))
	{
		m_tank->Move(ENTITY_DIRECTION_LEFT);
	}
	if (KEYSTATE(HGEK_D))
	{
		m_tank->Move(ENTITY_DIRECTION_RIGHT);
	}
	if (KEYSTATE(HGEK_SPACE))
	{
		if (m_firecd->ReachCycleStart())
		{
			m_tank->Fire();
			AudioManager::GetInstance()->PlayEffect(m_efffire);
		}
	}
	/*if (KEYSTATE(HGEK_R))
	{
		m_tank->SetHP(100);
	}*/
}