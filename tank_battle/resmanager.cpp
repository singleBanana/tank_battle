#include "resmanager.h"
#include "helpermacro.h"

ResManager* ResManager::m_instance = nullptr;

ResManager::ResManager()
{

}

ResManager::~ResManager()
{
	SAFE_DELETE(m_resmanager);
}

ResManager* ResManager::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new ResManager;
		m_instance->m_resmanager = new hgeResourceManager(GAMEPLAY_RESSCRIPTNAME);
		if (!m_instance->m_resmanager->Precache())
		{
			return nullptr;
		}
	}

	return m_instance;
}


HTEXTURE ResManager::GetTexture(const char* name)
{
	return m_resmanager->GetTexture(name);
}

hgeSprite* ResManager::GetSprite(const char* name)
{
	return m_resmanager->GetSprite(name);
}

hgeFont* ResManager::GetFont(const char* name)
{
	return m_resmanager->GetFont(name);
}

HEFFECT ResManager::GetEffect(const char* name)
{
	return m_resmanager->GetEffect(name);
}

hgeParticleSystem* ResManager::GetParticleSystem(const char* name)
{
	return m_resmanager->GetParticleSystem(name);
}