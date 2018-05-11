#ifndef _AI_STATEMACHINE_H
#define _AI_STATEMACHINE_H

#include "ai_base.h"
#include "helperclass.h"

#include <vector>
#include <string>
#include <stack>
#include <queue>
using namespace std;

#define AI_SM_THINK_T 30
#define AI_SM_SIGHT_LEN 8

enum AI_STATEMACHINE
{
	AI_STATEMACHINE_PATROL,
	AI_STATEMACHINE_ATTACK,
	AI_STATEMACHINE_ESCAPE
};

class AI_StateMachine:public AI_Base
{
	Time_Cycle* m_timecycle;
	AI_STATEMACHINE m_curstate;
	ENTITY_DIRECTION m_curdir;

	ENTITY_DIRECTION m_adjustdir;

	std::vector<Tank*> m_allenemy;
	Tank* m_targetenemy;

	string m_statusstr;

	//attack
	bool m_attack_tracewaydone;
	stack<int> m_attack_traceway;
	pair<int, int> m_attack_nextmovepos;
	bool m_attack_moveflag;

	//escape
	queue<ENTITY_DIRECTION> m_escape_dir;

public:
	AI_StateMachine(Tank* tank);

	void AddEnemy(Tank* enemy);

	void Think();

	void AdjustPos(); //脱离碰撞

	bool IsDisadvantage();

	void SetToPatrol();
	void SetToAttack();
	void SetToEscape();

	void DoPatrol();
	void DoAttack();
	void DoEscape();

	void OnContact(Entity* ent);

	bool MoveTo_FX(int x, int y); //x方向优先
	bool MoveTo_FY(int x, int y); //y方向优先

	//patrol
	bool EnemyInSight();
	void GetSightRect(vector<RECT>& out);
	void CalFreeDir(vector<ENTITY_DIRECTION>& out);

	//attack
	bool EnemyInAttackSight();
	RECT GetAttackSightRect();

	//escape
	void GenEscapeDir();
};

#endif