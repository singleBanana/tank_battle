#include "ai_base.h"

AI_Base::AI_Base(Tank* tank)
{
	m_tank = tank;
}

AI_Base::~AI_Base()
{

}

Tank* AI_Base::GetTank()
{
	return m_tank;
}

void AI_Base::OnContact(Entity* ent)
{

}