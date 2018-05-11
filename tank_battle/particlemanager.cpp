#include "particlemanager.h"
#include "gameplay.h"
#include "resmanager.h"
#include "helpermacro.h"

ParticleManager* ParticleManager::m_instance = nullptr;

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{
	SAFE_DELETE(m_particlemanager);
}

ParticleManager* ParticleManager::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new ParticleManager;
		m_instance->m_particlemanager = new hgeParticleManager;
	}

	return m_instance;
}

bool ParticleManager::SpawnParticle(char* partname, float x, float y) //x,y为粒子的中心坐标
{
	hgeParticleSystem* ps = ResManager::GetInstance()->GetParticleSystem(partname);
	if (ps)
	{
		if (m_particlemanager->SpawnPS(&ps->info, x, y))
		{
			return true;
		}
	}

	return false;
}

bool ParticleManager::SpawnParticleFollowEntity(char* partname, Entity* ent, bool tail)
{
	if (!EntityManager::GetInstance()->IsValidEntity(ent))
	{
		return false;
	}

	hgeParticleSystem* ps = ResManager::GetInstance()->GetParticleSystem(partname);
	if (ps)
	{
		hgeParticleSystem* psspawned = m_particlemanager->SpawnPS(&ps->info, ent->GetPositon().x, ent->GetPositon().y);
		if (psspawned)
		{
			MovePartData mpd;
			mpd.ps = psspawned;
			mpd.ent = ent;
			mpd.tail = tail;
			m_movepartlist.push_back(mpd);

			return true;
		}
	}

	return false;
}

void ParticleManager::UpdateParticle()
{
	for (auto it = m_movepartlist.begin(); it != m_movepartlist.end(); )
	{
		MovePartData mpd = (*it);
		if (!EntityManager::GetInstance()->IsValidEntity(mpd.ent))
		{
			it = m_movepartlist.erase(it);
		}
		else
		{
			mpd.ps->MoveTo(mpd.ent->GetPositon().x, mpd.ent->GetPositon().y, !mpd.tail);

			++it;
		}
	}

	m_particlemanager->Update(GamePlay::GetInstance()->GetDelta());
}

void ParticleManager::RenderParticle()
{
	m_particlemanager->Render();
}

void ParticleManager::KillParticle(hgeParticleSystem* ps)
{
	m_particlemanager->KillPS(ps);

	for (auto it = m_movepartlist.begin(); it != m_movepartlist.end(); ++it)
	{
		if ((*it).ps == ps)
		{
			m_movepartlist.erase(it);

			break;
		}
	}
}

void ParticleManager::KillParticleFromFollowedEnt(Entity* ent)
{
	for (auto it = m_movepartlist.begin(); it != m_movepartlist.end(); ++it)
	{
		if ((*it).ent == ent)
		{
			m_particlemanager->KillPS((*it).ps);
			m_movepartlist.erase(it);

			break;
		}
	}
}

void ParticleManager::KillAllParticle()
{
	m_particlemanager->KillAll();

	m_movepartlist.clear();
}