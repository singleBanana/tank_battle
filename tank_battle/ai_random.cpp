#include "ai_random.h"
#include "helpermacro.h"

AI_Random::AI_Random(Tank* tank) : AI_Base(tank)
{
	m_movecd = new Time_Cycle(AI_RAND_CD_MOVE);
	m_firecd = new Second_Cycle(AI_RAND_CD_FIRE);
}

AI_Random::~AI_Random()
{
	SAFE_DELETE(m_movecd);
	SAFE_DELETE(m_firecd);
}

void AI_Random::Think()
{
	if (m_movecd->ReachCycleStart()) //新的周期开始,选择移动方向
	{
		m_curdir = RandDir();
		m_tank->Move(m_curdir);
	}
	else //周期中
	{
		m_tank->Move(m_curdir);
	}

	if (HitChance(1, 30))
	{
		if (m_firecd->ReachCycleStart())
		{
			m_tank->Fire();
		}
	}
}