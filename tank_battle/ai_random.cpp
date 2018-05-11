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
	if (m_movecd->ReachCycleStart()) //�µ����ڿ�ʼ,ѡ���ƶ�����
	{
		m_curdir = RandDir();
		m_tank->Move(m_curdir);
	}
	else //������
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