#ifndef _AI_RANDOM_H
#define _AI_RANDOM_H

#include "ai_base.h"
#include "helperclass.h"

#define AI_RAND_CD_MOVE 20
#define AI_RAND_CD_FIRE 0.2

class AI_Random:public AI_Base
{
	Time_Cycle* m_movecd;
	Second_Cycle* m_firecd;
	ENTITY_DIRECTION m_curdir;

public:
	AI_Random(Tank* tank);
	~AI_Random();

	void Think();
};

#endif