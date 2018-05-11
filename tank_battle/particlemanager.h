#ifndef _PARTICLEMANAGER_H
#define _PARTICLEMANAGER_H

#include <hgeparticle.h>

#include "entity.h"

#include <list>

class ParticleManager
{
	struct MovePartData
	{
		hgeParticleSystem* ps;
		Entity* ent;
		bool tail;
	};

	static ParticleManager* m_instance;

	hgeParticleManager* m_particlemanager;
	std::list<MovePartData> m_movepartlist;

	ParticleManager();

public:
	~ParticleManager();

	static ParticleManager* GetInstance();

	bool SpawnParticle(char* partname, float x, float y); //x,y为粒子的中心坐标
	bool SpawnParticleFollowEntity(char* partname, Entity* ent, bool tail = true);
	void UpdateParticle();
	void RenderParticle();
	void KillParticle(hgeParticleSystem* ps);
	void KillParticleFromFollowedEnt(Entity* ent);
	void KillAllParticle();
};

#endif