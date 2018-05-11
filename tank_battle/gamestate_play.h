#ifndef _GAMESTATE_PLAY_H
#define _GAMESTATE_PLAY_H

#include "gamestate.h"
#include "box2ddef.h"
#include "helpermacro.h"
#include "helperclass.h"
#include "tank.h"
#include "tank_enemy.h"
#include "block.h"
#include "meshmap.h"
#include "ai.h"
#include "bouns.h"

#include <string>
#include <memory>

#define BLOCK_HP_BASE 50
#define POINT_KILL_BLOCK 1
#define POINT_HIT_BOUNS 5

#define ENEMY_SPAWN_CD 60

#define BOUNS_LAST_TIME 300

#define LEVEL_START "level_start"

typedef std::function<void(void* data, bool isoncleancall)> gsplaysectimercallback;

class GameState_Play : public GameState
{
	enum GAMESTATE_PLAY_STATUS
	{
		GAMESTATE_PLAY_STATUS_PROGRESSING,
		GAMESTATE_PLAY_STATUS_WIN,
		GAMESTATE_PLAY_STATUS_LOSE
	};

	struct EnemyTankInfo
	{
		int posx;
		int posy;
		int teamid;
		TANK_ENEMY_TYPE type;

		EnemyTankInfo(int x, int y, int tid, TANK_ENEMY_TYPE t);
	};

	struct SecTimerData
	{
		Second_Cycle* sc;
		gsplaysectimercallback callback;
		void* data;
		bool oncleancall;
	};

	bool m_solo;
	string m_mapname;
	string m_curlevel;
	string m_nextlevel;
	string m_levelname;

	bool m_pausegame;

	std::auto_ptr<EntityManager> m_entmanager;
//	std::auto_ptr<MeshMap> m_meshmap;

	Tank* m_player;

	std::vector<Block*> m_blocks;

	int m_enemymaxnum;
	int m_enemyspawnednum;
	int m_enemymaxsametimespawn;
	std::vector<Tank_Enemy*> m_enemy;
	std::vector<EnemyTankInfo> m_enemyinfo;
	Time_Cycle* m_enemyspawntimer;

	std::vector<AI_Base*> m_ai;

	Time_Cycle* m_bounstimer;
	Bouns_Base* m_bouns;

	int m_point;
	GAMESTATE_PLAY_STATUS m_gamestatus;

	list<SecTimerData> m_sectimer; //Ò»´ÎÐÔ

	HEFFECT m_effbullethittank;
	HEFFECT m_effbullethitblock;
	HEFFECT m_effbullethitbound;
	HEFFECT m_efftankdestroy;
	HEFFECT m_effblockdestroy;
	HEFFECT m_effbounshit;
	HEFFECT m_effgamepause;
	HEFFECT m_effgamewin;
	HEFFECT m_effgamelose;
	HEFFECT m_effbgm;

	hgeSprite* m_bgspr;

public:
	GameState_Play();
	~GameState_Play();

	void Clean();
	void CleanGameElement();
	bool ResetGame(bool curlevel = false);

	void AddSecTimerCallBack(gsplaysectimercallback callback, float t, void* data = nullptr, bool oncleancall = false);

	Tank* GetPlayer();
	int GetPoint();
	void SetPoint(int p);

	void SetSolo(bool solo);
	bool LoadMap(const char* filename, const char* mapname);

	void OnEntityContact(Vec2b2fixture vecb2fix);

	void Render();
	bool Think();
	void OnEnterState();

	//struct AIInfo
	//{
	//	Tank* tank;
	//	AI_Base* ai;
	//
	//	AIInfo()
	//	{
	//		tank = nullptr;
	//		ai = nullptr;
	//	};
	//
	//	AIInfo(Tank* t, AI_Base* a)
	//	{
	//		tank = t;
	//		ai = a;
	//	};
	//
	//	bool operator ==(const AIInfo& other) const
	//	{
	//		if (tank == other.tank && ai == other.ai)
	//		{
	//			return true;
	//		}
	//
	//		return false;
	//	};
	//};
};

#endif