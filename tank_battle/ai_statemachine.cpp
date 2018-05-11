#include "ai_statemachine.h"
#include "meshmap.h"
#include "gameplay.h"

AI_StateMachine::AI_StateMachine(Tank* tank) :AI_Base(tank)
{
	m_timecycle = new Time_Cycle(AI_SM_THINK_T);
	m_adjustdir = ENTITY_DIRECTION_UNDEFINE;

	SetToPatrol();
}

void AI_StateMachine::Think()
{
	if (m_timecycle->ReachCycleStart())
	{
		m_statusstr.clear();

		if (m_adjustdir != ENTITY_DIRECTION_UNDEFINE)
		{
			m_statusstr += "adjustpos ";
			AdjustPos();
		}

		switch (m_curstate)
		{
			case AI_STATEMACHINE_PATROL:
			{
				m_statusstr += "patrol\n";
				DoPatrol();

				break;
			}
			case AI_STATEMACHINE_ATTACK:
			{
				m_statusstr += "attack\n";
				DoAttack();

				break;
			}
			case AI_STATEMACHINE_ESCAPE:
			{
				m_statusstr += "escape\n";
				DoEscape();

				break;
			}
		}
	}
	else
	{
		GamePlay::GetInstance()->PrepareRenderString(m_statusstr.c_str(), 10, 80);
	}
}

void AI_StateMachine::AdjustPos()
{
	m_tank->Move(m_adjustdir, 1);
	m_tank->Move(m_curdir, 0); //还原方向
	m_adjustdir = ENTITY_DIRECTION_UNDEFINE;
}

bool AI_StateMachine::IsDisadvantage()
{
	if (m_targetenemy->GetHP() - m_tank->GetHP() > 20)
	{
		return true;
	}

	return false;
}

void AI_StateMachine::SetToPatrol()
{
	m_curstate = AI_STATEMACHINE_PATROL;
	m_curdir = RandDir();

	m_targetenemy = nullptr;
}

void AI_StateMachine::SetToAttack()
{
	m_curstate = AI_STATEMACHINE_ATTACK;

	m_attack_tracewaydone = false;
	m_attack_moveflag = true;
}

void AI_StateMachine::SetToEscape()
{
	m_curstate = AI_STATEMACHINE_ESCAPE;
}

void AI_StateMachine::AddEnemy(Tank* enemy)
{
	m_allenemy.push_back(enemy);
}

void AI_StateMachine::DoPatrol()
{
	m_tank->Move(m_curdir);

	if (EnemyInSight())
	{
		m_statusstr += "enemy find!\n";

		if (IsDisadvantage())
		{
			SetToEscape();
		}
		else
		{
			SetToAttack();
		}
	}
	else
	{
		m_statusstr += "no enemy\n";
	}
}

void AI_StateMachine::DoAttack()
{
	MeshMap* mmptr = MeshMap::GetInstance();
	if (!m_attack_tracewaydone)
	{
		m_statusstr += "A*\n";

		vector<int> traceway;
		b2Vec2 tankpos = m_tank->GetPositon();
		b2Vec2 targetpos = m_targetenemy->GetPositon();
		mmptr->AStar(traceway, mmptr->Pos2ID(tankpos.x, tankpos.y), mmptr->Pos2ID(targetpos.x, targetpos.y));

		for (int i = 0; i < traceway.size(); ++i)
		{
			m_attack_traceway.push(traceway[i]);
		}
		m_attack_traceway.pop(); //去掉出发点

		m_attack_tracewaydone = true;
		m_attack_nextmovepos = mmptr->ID2Pos(m_attack_traceway.top());
	}
	else
	{
		if (EnemyInAttackSight())
		{
			m_tank->Fire();
		}

		bool moveret;
		if (m_attack_moveflag)
		{
			moveret = MoveTo_FX(m_attack_nextmovepos.first, m_attack_nextmovepos.second);
		}
		else
		{
			moveret = MoveTo_FY(m_attack_nextmovepos.first, m_attack_nextmovepos.second);
		}

		if (!moveret)
		{
			m_attack_traceway.pop();
			if (m_attack_traceway.size())
			{
				m_attack_nextmovepos = mmptr->ID2Pos(m_attack_traceway.top());
			}
			else
			{
				SetToPatrol();
			}
		}
	}
}

void AI_StateMachine::DoEscape()
{
	if (m_escape_dir.empty())
	{
		m_statusstr += "create escape direction\n";
		GenEscapeDir();
	}
	else
	{
		m_tank->Move(m_escape_dir.front());
	}
}

void AI_StateMachine::OnContact(Entity* ent)
{
	//下一次think脱离碰撞
	if (ent->GetType() == "block")
	{
		m_adjustdir = GetOppositeDir(m_curdir);
	}

	switch (m_curstate)
	{
		case AI_STATEMACHINE_PATROL:
		{
			if (ent->GetType() == "block")
			{
				vector<ENTITY_DIRECTION> freedir;
				CalFreeDir(freedir);
				m_curdir = freedir[0];
			}

			break;
		}
		case AI_STATEMACHINE_ATTACK:
		{
			if (ent->GetType() == "block")
			{
				m_attack_moveflag = !m_attack_moveflag;
			}

			break;
		}
		case AI_STATEMACHINE_ESCAPE:
		{
			if (ent->GetType() == "block")
			{
				m_escape_dir.pop();
				if (m_escape_dir.empty())
				{
					SetToPatrol();
				}
			}

			break;
		}
	}
}

bool AI_StateMachine::MoveTo_FX(int x, int y)
{
	b2Vec2 pos = m_tank->GetPositon();
	int px = pos.x;
	int py = pos.y;
	int dx = abs(px - x);
	int dy = abs(py - y);
	int movespeed = m_tank->GetSpeed();
	ENTITY_DIRECTION dir = ENTITY_DIRECTION_UNDEFINE;
	if (px < x)
	{
		if (dx <= movespeed)
		{
			movespeed = dx;
		}
		dir = ENTITY_DIRECTION_RIGHT;
	}
	else if (px > x)
	{
		if (dx <= movespeed)
		{
			movespeed = dx;
		}
		dir = ENTITY_DIRECTION_LEFT;
	}
	else
	{
		if (py < y)
		{
			if (dy <= movespeed)
			{
				movespeed = dy;
			}
			dir = ENTITY_DIRECTION_DOWN;
		}
		else if (py > y)
		{
			if (dy <= movespeed)
			{
				movespeed = dy;
			}
			dir = ENTITY_DIRECTION_UP;
		}
	}

	if (dir != ENTITY_DIRECTION_UNDEFINE)
	{
		m_curdir = dir;
		m_tank->Move(m_curdir, movespeed);

		return true;
	}

	return false;
}

bool AI_StateMachine::MoveTo_FY(int x, int y)
{
	b2Vec2 pos = m_tank->GetPositon();
	int px = pos.x;
	int py = pos.y;
	int dx = abs(px - x);
	int dy = abs(py - y);
	int movespeed = m_tank->GetSpeed();
	ENTITY_DIRECTION dir = ENTITY_DIRECTION_UNDEFINE;
	if (py < y)
	{
		if (dy <= movespeed)
		{
			movespeed = dy;
		}
		dir = ENTITY_DIRECTION_DOWN;
	}
	else if (py > y)
	{
		if (dy <= movespeed)
		{
			movespeed = dy;
		}
		dir = ENTITY_DIRECTION_UP;
	}
	else
	{
		if (px < x)
		{
			if (dx <= movespeed)
			{
				movespeed = dx;
			}
			dir = ENTITY_DIRECTION_RIGHT;
		}
		else if (px > x)
		{
			if (dx <= movespeed)
			{
				movespeed = dx;
			}
			dir = ENTITY_DIRECTION_LEFT;
		}
	}

	if (dir != ENTITY_DIRECTION_UNDEFINE)
	{
		m_curdir = dir;
		m_tank->Move(m_curdir, movespeed);

		return true;
	}

	return false;
}

bool AI_StateMachine::EnemyInSight()
{
	vector<RECT> sightrect;
	GetSightRect(sightrect);

	char temp[128];
	sprintf(temp, "%f %f\n", m_allenemy[0]->GetPositon().x, m_allenemy[0]->GetPositon().y);
	m_statusstr += temp;
	sprintf(temp, "%d %d %d %d\n", sightrect[0].left, sightrect[0].right, sightrect[0].top, sightrect[0].bottom);
	m_statusstr += temp;
	sprintf(temp, "%d %d %d %d\n", sightrect[1].left, sightrect[1].right, sightrect[1].top, sightrect[1].bottom);
	m_statusstr += temp;
	sprintf(temp, "%d %d %d %d\n", sightrect[2].left, sightrect[2].right, sightrect[2].top, sightrect[2].bottom);
	m_statusstr += temp;

	bool hasenemy = false;
	for (int i = 0; i < sightrect.size(); ++i)
	{
		for (int j = 0; j < m_allenemy.size(); ++j)
		{
			b2Vec2 enemypos = m_allenemy[j]->GetPositon();
			if (IsPointInRect(enemypos.x, enemypos.y, sightrect[i]))
			{
				m_targetenemy = m_allenemy[j];
				hasenemy = true;

				break;
			}
			if (hasenemy)
			{
				break;
			}
		}
	}

	return hasenemy;
}

void AI_StateMachine::GetSightRect(vector<RECT>& out)
{
	out.clear();

	vector<int> vecstartid;
	MeshMap* mmptr = MeshMap::GetInstance();
	vector<int> aroundid;
	b2Vec2 tankpos = m_tank->GetPositon();
	mmptr->GetAroundID(aroundid, mmptr->Pos2ID(tankpos.x, tankpos.y));

	switch (m_curdir)
	{
		case ENTITY_DIRECTION_UP:
		{
			vecstartid.push_back(aroundid[0]);
			vecstartid.push_back(aroundid[1]);
			vecstartid.push_back(aroundid[2]);

			for (int i = 0; i < vecstartid.size(); ++i)
			{
				out.push_back(mmptr->GetUnBlockedRect_Col(vecstartid[i], -AI_SM_SIGHT_LEN));
			}

			break;
		}
		case ENTITY_DIRECTION_LEFT:
		{
			vecstartid.push_back(aroundid[0]);
			vecstartid.push_back(aroundid[3]);
			vecstartid.push_back(aroundid[5]);

			for (int i = 0; i < vecstartid.size(); ++i)
			{
				out.push_back(mmptr->GetUnBlockedRect_Row(vecstartid[i], -AI_SM_SIGHT_LEN));
			}

			break;
		}
		case ENTITY_DIRECTION_DOWN:
		{
			vecstartid.push_back(aroundid[5]);
			vecstartid.push_back(aroundid[6]);
			vecstartid.push_back(aroundid[7]);

			for (int i = 0; i < vecstartid.size(); ++i)
			{
				out.push_back(mmptr->GetUnBlockedRect_Col(vecstartid[i], AI_SM_SIGHT_LEN));
			}

			break;
		}
		case ENTITY_DIRECTION_RIGHT:
		{
			vecstartid.push_back(aroundid[2]);
			vecstartid.push_back(aroundid[4]);
			vecstartid.push_back(aroundid[7]);

			for (int i = 0; i < vecstartid.size(); ++i)
			{
				out.push_back(mmptr->GetUnBlockedRect_Row(vecstartid[i], AI_SM_SIGHT_LEN));
			}

			break;
		}
	}
}

void AI_StateMachine::CalFreeDir(vector<ENTITY_DIRECTION>& out)
{
	out.clear();

	MeshMap* mm = MeshMap::GetInstance();
	vector<int> aid;
	mm->GetAroundID(aid, mm->Pos2ID(m_tank->GetPositon().x, m_tank->GetPositon().y));

	if (aid[1] != -1 && mm->GetCost(aid[1]) != COST_BLOCK)
	{
		out.push_back(ENTITY_DIRECTION_UP);
	}
	if (aid[3] != -1 && mm->GetCost(aid[3]) != COST_BLOCK)
	{
		out.push_back(ENTITY_DIRECTION_LEFT);
	}
	if (aid[4] != -1 && mm->GetCost(aid[4]) != COST_BLOCK)
	{
		out.push_back(ENTITY_DIRECTION_RIGHT);
	}
	if (aid[6] != -1 && mm->GetCost(aid[6]) != COST_BLOCK)
	{
		out.push_back(ENTITY_DIRECTION_DOWN);
	}
}

bool AI_StateMachine::EnemyInAttackSight()
{
	RECT attackrect = GetAttackSightRect();

	char temp[128];
	sprintf(temp, "%f %f\n", m_targetenemy->GetPositon().x, m_targetenemy->GetPositon().y);
	m_statusstr += temp;
	sprintf(temp, "%d %d %d %d\n", attackrect.left, attackrect.right, attackrect.top, attackrect.bottom);
	m_statusstr += temp;

	b2Vec2 enemypos = m_targetenemy->GetPositon();
	if (IsPointInRect(enemypos.x, enemypos.y, attackrect))
	{
		m_statusstr += "fire!\n";

		return true;
	}

	m_statusstr += "no enemy\n";

	return false;
}

RECT AI_StateMachine::GetAttackSightRect()
{
	RECT result;
	result.top = -1;
	result.left = -1;
	result.bottom = -1;
	result.right = -1;
	MeshMap* mmptr = MeshMap::GetInstance();
	vector<int> aroundid;
	b2Vec2 tankpos = m_tank->GetPositon();
	mmptr->GetAroundID(aroundid, mmptr->Pos2ID(tankpos.x, tankpos.y));

	switch (m_curdir)
	{
		case ENTITY_DIRECTION_UP:
		{
			result = mmptr->GetUnBlockedRect_Col(aroundid[1], -AI_SM_SIGHT_LEN);

			break;
		}
		case ENTITY_DIRECTION_LEFT:
		{
			result = mmptr->GetUnBlockedRect_Row(aroundid[3], -AI_SM_SIGHT_LEN);

			break;
		}
		case ENTITY_DIRECTION_DOWN:
		{
			result = mmptr->GetUnBlockedRect_Col(aroundid[6], AI_SM_SIGHT_LEN);

			break;
		}
		case ENTITY_DIRECTION_RIGHT:
		{
			result = mmptr->GetUnBlockedRect_Row(aroundid[4], AI_SM_SIGHT_LEN);

			break;
		}
	}

	return result;
}

void AI_StateMachine::GenEscapeDir()
{
	m_escape_dir.push(GetOppositeDir(GetDirBetweenEnt(m_tank, m_targetenemy)));

	vector<ENTITY_DIRECTION> sidedir;
	GetSideDir(sidedir, m_escape_dir.front());
	m_escape_dir.push(sidedir[0]);
	m_escape_dir.push(sidedir[1]);
}