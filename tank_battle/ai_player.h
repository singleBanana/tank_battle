#ifndef _AI_PLAYER_H
#define _AI_PLAYER_H

#include "ai_base.h"
#include "helperclass.h"

#define AI_PLAYER_CD_FIRE 0.2

class AI_Player :public AI_Base
{
	Second_Cycle* m_firecd;
	HEFFECT m_efffire;

public:
	AI_Player(Tank* tank);
	~AI_Player();

	void Think();
};

#endif