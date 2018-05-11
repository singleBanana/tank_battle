#include "gamestate_play.h"
#include "gamestatemanager.h"
#include "audiomanager.h"
#include "resmanager.h"
#include "particlemanager.h"

GameState_Play::EnemyTankInfo::EnemyTankInfo(int x, int y, int tid, TANK_ENEMY_TYPE t)
{
	posx = x;
	posy = y;
	teamid = tid;
	type = t;
};

GameState_Play::GameState_Play()
	:m_entmanager(EntityManager::GetInstance())/*, m_meshmap(MeshMap::GetInstance())*/
{
	m_solo = true;

	do
	{
		ResManager* rm = ResManager::GetInstance();

		//entity manager
		m_entmanager->InitWorld();
		m_entmanager->AddContactCallback(FUNC_CALLBACK_1(GameState_Play::OnEntityContact, this));
		m_entmanager->SetDebugDraw(false);

		//meshmap
		/*MapPtr mapptr = gp->GetMapPTR(m_mapname.c_str());
		BREAKIF(!mapptr);
		m_meshmap->Init(mapptr->GetTotalMapWidth(), mapptr->GetTotalMapHeight(), 32, 32);*/

		//timer
		m_enemyspawntimer = new Time_Cycle(ENEMY_SPAWN_CD);
		m_bounstimer = new Time_Cycle(BOUNS_LAST_TIME);

		//effect
		m_effbullethittank = rm->GetEffect("bullet_hit_tank");
		m_effbullethitblock = rm->GetEffect("bullet_hit_block");
		m_effbullethitbound = rm->GetEffect("bullet_hit_bound");
		m_efftankdestroy = rm->GetEffect("tank_destroy");
		m_effblockdestroy = rm->GetEffect("block_destroy");
		m_effbounshit = rm->GetEffect("bouns_hit");
		m_effgamepause = rm->GetEffect("game_pause");
		m_effgamewin = rm->GetEffect("game_win");
		m_effgamelose = rm->GetEffect("game_lose");
		m_effbgm = rm->GetEffect("bgm_play");

		//bg
		m_bgspr = rm->GetSprite("spr_bg_play");

	} while (false);
}

GameState_Play::~GameState_Play()
{
	Clean();
}

void GameState_Play::Clean()
{
	CleanGameElement();

	SAFE_DELETE(m_enemyspawntimer);
	SAFE_DELETE(m_bounstimer);
}

void GameState_Play::CleanGameElement()
{
	EntityManager::GetInstance()->DestroyAllEntity();

	m_player = nullptr;
	m_blocks.clear();
	m_enemy.clear();
	m_enemyinfo.clear();

	for (int i = 0; i < m_ai.size(); ++i)
	{
		SAFE_DELETE(m_ai[i]);
	}
	m_ai.clear();

	m_bouns = nullptr;

	m_point = 0;
	m_enemymaxnum = 0;
	m_enemyspawnednum = 0;
	m_enemymaxsametimespawn = 0;
	m_pausegame = false;
	m_gamestatus = GAMESTATE_PLAY_STATUS_PROGRESSING;

	for (auto it = m_sectimer.begin(); it != m_sectimer.end(); ++it)
	{
		SecTimerData sectdata = *it;
		if (sectdata.oncleancall)
		{
			sectdata.callback(sectdata.data, true);
		}
		SAFE_DELETE(sectdata.sc);
	}
	m_sectimer.clear();

	ParticleManager::GetInstance()->KillAllParticle();
}

bool GameState_Play::ResetGame(bool curlevel)
{
	CleanGameElement();

	bool initdone = false;
	do
	{
		GamePlay* gp = GamePlay::GetInstance();
		ResManager* rm = ResManager::GetInstance();

		//level
		string mapfilename;
		if (!curlevel)
		{
			if (m_curlevel.empty()) //开始
			{
				m_curlevel = LEVEL_START;
			}
			else
			{
				m_curlevel = m_nextlevel;
			}
		}
		mapfilename += m_curlevel;
		mapfilename += ".tmx";
		BREAKIF(!LoadMap(mapfilename.c_str(), "level"));
		m_nextlevel = gp->GetLayerProperty(m_mapname.c_str(), "info", "nextlevel");
		m_levelname = gp->GetLayerProperty(m_mapname.c_str(), "info", "levelname");

		//player
		ObjectPtr playerobj = gp->GetObjectInTile(m_mapname.c_str(), "player", "spawn");
		BREAKIF(!playerobj);
		int playermaxhp = atoi(gp->GetLayerProperty(m_mapname.c_str(), "player", "maxhp"));
		std::pair<int, int> playerpos = playerobj->GetPosition();
		m_player = new Tank(playerpos.first, playerpos.second, 32, 32, 0);
		m_player->SetTexture(rm->GetTexture("player"));
		m_player->SetMaxHP(playermaxhp);
		m_player->SetHP(playermaxhp);
		//for test
		/*m_player->SetInvincible(true);
		m_player->SetBulletPower(9999);*/

		//block
		std::vector<ObjectPtr> blocks;
		BREAKIF(!gp->GetObjectInLayer(m_mapname.c_str(), "block", blocks));
		for (int i = 0; i < blocks.size(); ++i)
		{
			std::pair<int, int> blockpos = blocks[i]->GetPosition();
			int blockx = blockpos.first + 1; //适当减小大小防止动态实体卡在两者之间滑动
			int blocky = blockpos.second + 1;
			int blockw = blocks[i]->GetWidth() - 2;
			int blockh = blocks[i]->GetHeight() - 2;
			Block* blockent = new Block(blockx, blocky, blockw, blockh);

			if (blocks[i]->GetProperty("mapbound"))
			{
				blockent->SetTexture(rm->GetTexture("mapbound"));
				blockent->SetInvincible(true);
			}
			else
			{
				blockent->SetTexture(rm->GetTexture("block"));
			}
			int bmaxhp = (blocks[i]->GetWidth() / 32) * (blocks[i]->GetHeight() / 32) * BLOCK_HP_BASE;
			blockent->SetMaxHP(bmaxhp);
			blockent->SetHP(bmaxhp);
			m_blocks.push_back(blockent);

			//meshmap
			//	m_meshmap->AddBlockLocation(GenGeoDesc(blockent));
		}

		//enemy
		m_enemymaxsametimespawn = atoi(gp->GetLayerProperty(m_mapname.c_str(), "enemyspawn", "sametimespawnnum"));
		std::vector<ObjectPtr> enemys;
		BREAKIF(!gp->GetObjectInLayer(m_mapname.c_str(), "enemyspawn", enemys));
		m_enemymaxnum = atoi(gp->GetLayerProperty(m_mapname.c_str(), "enemyspawn", "enemynum"));
		for (int i = 0; i < enemys.size(); ++i)
		{
			std::pair<int, int> pos = enemys[i]->GetPosition();
			int teamid = atoi(enemys[i]->GetProperty("team")->GetValue());
			int enemytype = atoi(enemys[i]->GetProperty("type")->GetValue());
			m_enemyinfo.push_back(EnemyTankInfo(pos.first, pos.second, teamid, (TANK_ENEMY_TYPE)enemytype));
		}

		//ai
		//	AI_Random* randai = new AI_Random(enemy);
		//	g_ai.push_back(randai);

		/*AI_StateMachine* smai = new AI_StateMachine(enemy);
		smai->AddEnemy(g_player);
		aiinfo.tank = enemy;
		aiinfo.ai = smai;
		g_ai.push_back(aiinfo);
		g_smai.push_back(smai);*/

		AI_Player* playerai = new AI_Player(m_player);
		m_ai.push_back(playerai);

		//bouns
		m_bouns = RandBouns();

		initdone = true;

	} while (false);

	return initdone;
}

void GameState_Play::AddSecTimerCallBack(gsplaysectimercallback callback, float t, void* data, bool oncleancall)
{
	SecTimerData stdata;
	stdata.sc = new Second_Cycle(t);
	stdata.callback = callback;
	stdata.data = data;
	stdata.oncleancall = oncleancall;
	m_sectimer.push_back(stdata);
}

Tank* GameState_Play::GetPlayer()
{
	return m_player;
}

int GameState_Play::GetPoint()
{
	return m_point;
}

void GameState_Play::SetPoint(int p)
{
	m_point = p;
}

void GameState_Play::SetSolo(bool solo)
{
	m_solo = solo;
}

bool GameState_Play::LoadMap(const char* filename, const char* mapname)
{
	bool ret = GamePlay::GetInstance()->LoadTileMap(filename, mapname);
	if (ret)
	{
		m_mapname = mapname;
	}

	return ret;
}

void GameState_Play::OnEntityContact(Vec2b2fixture vecb2fix)
{
	Entity* ent1 = (Entity*)vecb2fix.f1->GetBody()->GetUserData();
	Entity* ent2 = (Entity*)vecb2fix.f2->GetBody()->GetUserData();

	if (!ent1 || !ent2)
	{
		return;
	}

	bool asc;
	GamePlay* gp = GamePlay::GetInstance();
	AudioManager* am = AudioManager::GetInstance();
	ResManager* rm = ResManager::GetInstance();

	//ai
	if (EntTypeHas("tank", ent1, ent2, asc))
	{
		Tank* tankent = (Tank*)(asc ? ent1 : ent2);
		Entity* otherent = asc ? ent2 : ent1;
		for (int i = 0; i < m_ai.size(); ++i)
		{
			if (m_ai[i]->GetTank() == tankent)
			{
				m_ai[i]->OnContact(otherent);

				break;
			}
		}
	}

	//bouns
	if (EntTypeHas("bouns", ent1, ent2, asc))
	{
		Bouns_Base* bouns = (Bouns_Base*)(asc ? ent1 : ent2);
		Entity* otherent = asc ? ent2 : ent1;
		if (otherent->GetType() == "tank")
		{
			bouns->OnHit((Tank*)otherent);

			SAFE_DESTROYENT(m_bouns);

			if (otherent == m_player)
			{
				m_point += POINT_HIT_BOUNS;
			}

			am->PlayEffect(m_effbounshit);

			return;
		}
	}

	//bullet
	if (EntTypeHas("bullet", ent1, ent2, asc))
	{
		Bullet* bullet = (Bullet*)(asc ? ent1 : ent2);
		Entity* other = asc ? ent2 : ent1;
		bool otheristank = (other->GetType() == "tank");
		bool otherisblock = (other->GetType() == "block");
		bool otherisbouns = (other->GetType() == "bouns");
		bool otherisplayer = (other == m_player);		
		bool playerbullet = (bullet->GetOwner() == m_player);

		if (!m_entmanager->IsValidEntity(bullet->GetOwner()))
		{
			SAFE_DESTROYENT(bullet);
			return;
		}

		if (otheristank && bullet->GetOwner()->GetTeam() == ((Tank*)other)->GetTeam())
		{
			//打中自己人 什么也不做
		}
		else
		{
			if (!other->Hit(bullet->GetAttack())) //没打死
			{
				if (otherisblock)
				{
					if (!other->IsInvincible())
					{
						am->PlayEffect(m_effbullethitblock);
					}
					else
					{
						am->PlayEffect(m_effbullethitbound);
					}
				}
				else if (otheristank)
				{
					am->PlayEffect(m_effbullethittank);
				}
			}
			else //打死
			{
				if (otheristank) //打死tank
				{
					//删除销毁ai
					for (auto it = m_ai.begin(); it != m_ai.end(); ++it)
					{
						if ((*it)->GetTank() == other)
						{
							SAFE_DELETE(*it);
							m_ai.erase(it);

							break;
						}
					}

					if (otherisplayer)
					{
						m_player = nullptr;
						m_gamestatus = GAMESTATE_PLAY_STATUS_LOSE;
						am->PlayEffect(m_effgamelose);
					}
					else
					{
						//删除enemy
						auto findit = std::find(m_enemy.begin(), m_enemy.end(), other);
						if (findit != m_enemy.end())
						{
							m_enemy.erase(findit);
						}

						if (playerbullet) //玩家打死
						{
							m_point += ((Tank_Enemy*)other)->GetDestroyPoint();
						}
					}

					am->PlayEffect(m_efftankdestroy);
				}
				else if (otherisblock) //打死block
				{
					//更新meshmap
				//	g_meshmap->DeleteBlockLocation(GenGeoDesc(other));

					//删除block
					auto findit = std::find(m_blocks.begin(), m_blocks.end(), other);
					if (findit != m_blocks.end())
					{
						m_blocks.erase(findit);
					}

					if (playerbullet)
					{
						m_point += POINT_KILL_BLOCK;
					}

					am->PlayEffect(m_effblockdestroy);
				}

				//销毁other
				SAFE_DESTROYENT(other);
			}
		}

		if (!otherisbouns)
		{
			ParticleManager::GetInstance()->SpawnParticle("bullet_hit", bullet->GetPositon().x, bullet->GetPositon().y);

			//销毁bullet
			SAFE_DESTROYENT(bullet);
		}

		return;
	}
}

void GameState_Play::Render()
{
	GamePlay* gp = GamePlay::GetInstance();

//	gp->RenderTile(m_mapname.c_str());
	m_bgspr->Render(0, 0);

	m_entmanager->Step(gp->GetDelta(), m_pausegame);

	gp->RenderFPS();
	gp->RenderCustomString();

	ParticleManager::GetInstance()->RenderParticle();
}

bool GameState_Play::Think()
{
	GamePlay* gp = GamePlay::GetInstance();
	AudioManager* am = AudioManager::GetInstance();

	//system
	if (KEYDOWN(HGEK_ESCAPE))
	{
		am->StopBGM();
		GameStateManager::GetInstance()->PopState();

		return false;
	}
	if (KEYDOWN(HGEK_ENTER))
	{
		if (m_gamestatus == GAMESTATE_PLAY_STATUS_WIN)
		{
			if (m_nextlevel.size()) //进入下一关
			{
				int prepoint = m_point;
				ResetGame();
				m_point = prepoint;
			}
			else //关卡全部打完
			{
				am->StopBGM();
				GameStateManager::GetInstance()->PopState();
			}

			return false;
		}
		else if (m_gamestatus == GAMESTATE_PLAY_STATUS_LOSE)
		{
			ResetGame(true);

			return false;
		}

		//进行中
		m_pausegame = !m_pausegame;
		if (m_pausegame)
		{
			am->PauseBGM();
		}
		else
		{
			am->ResumeBGM();
		}
		am->PlayEffect(m_effgamepause);
	}

	char temp[128];

	//info
	int playerhp = 0;
	int playerattack = 0;
	int enemyremain = m_enemymaxnum - m_enemyspawnednum + m_enemy.size();
	if (m_player)
	{
		playerhp = m_player->GetHP();
		playerattack = m_player->GetBulletPower();
	}
	sprintf(temp, "level: %s\nhp: %d  attack: %d\npoint: %d\nenemy remaining: %d\n", 
		m_levelname.c_str(), playerhp, playerattack, m_point, enemyremain);
	gp->PrepareRenderString(temp, 500, 10);

	if (!enemyremain && m_gamestatus != GAMESTATE_PLAY_STATUS_LOSE)
	{
		//win了
		if (m_gamestatus != GAMESTATE_PLAY_STATUS_WIN)
		{
			m_gamestatus = GAMESTATE_PLAY_STATUS_WIN;
			am->PlayEffect(m_effgamewin);
		}
	}

	//win/lose string
	switch (m_gamestatus)
	{
	case GAMESTATE_PLAY_STATUS_WIN:
	{
		if (m_nextlevel.empty()) //已经是最后一个图了
		{
			gp->PrepareRenderString("Level Clear!\nYou have cleared all the levels!\nPress Enter to main menu\n", 350, 200);
		}
		else
		{
			gp->PrepareRenderString("Level Clear!\nPress Enter to play next\n", 350, 200);
		}

		break;
	}
	case GAMESTATE_PLAY_STATUS_LOSE:
	{
		gp->PrepareRenderString("You Are Dead!\nPress Enter to restart\n", 350, 200);

		break;
	}
	}

	//pause game
	if (m_pausegame)
	{
		gp->PrepareRenderString("game paused", 350, 300);

		return false;
	}

	//ai
	for (int i = 0; i < m_ai.size(); ++i)
	{
		m_ai[i]->Think();
	}

	//enemyspawn
	if (m_enemyspawntimer->ReachCycleStart())
	{
		if (m_enemymaxsametimespawn - m_enemy.size() > 0) //同时还可以产生enemy
		{
			if (m_enemyspawnednum < m_enemymaxnum)
			{
				EnemyTankInfo enemyinfo = m_enemyinfo[Randint(0, m_enemyinfo.size() - 1)];
				Tank_Enemy* enemy = new Tank_Enemy(enemyinfo.posx, enemyinfo.posy, 32, 32, enemyinfo.teamid, enemyinfo.type);
				m_enemy.push_back(enemy);

				//ai
				AI_Random* randai = new AI_Random(enemy);
				m_ai.push_back(randai);

				++m_enemyspawnednum;
			}
		}
	}

	//bouns
	if (m_bounstimer->ReachCycleStart())
	{
		SAFE_DESTROYENT(m_bouns);
		m_bouns = RandBouns();
	}

	//particle
	ParticleManager::GetInstance()->UpdateParticle();

	//sectimer
	for (auto it = m_sectimer.begin(); it != m_sectimer.end();)
	{
		if ((*it).sc->ReachCycleStart(false))
		{
			(*it).callback((*it).data, false);
			SAFE_DELETE((*it).sc);
			it = m_sectimer.erase(it);
		}
		else
		{
			++it;
		}
	}

	return false;
}

void GameState_Play::OnEnterState()
{
	m_curlevel.clear();
	ResetGame();

	AudioManager::GetInstance()->PlayBGM(m_effbgm);
}