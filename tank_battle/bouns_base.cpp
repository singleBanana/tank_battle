#include "bouns_base.h"
#include "particlemanager.h"

Bouns_Base::Bouns_Base(float x, float y)
	:Entity(x, y, BOUNS_WIDTH, BOUNS_HEIGHT, 0, false, true, true, std::string("bouns"))
{
	SetInvincible(true);
}

Bouns_Base::~Bouns_Base()
{
	ParticleManager::GetInstance()->SpawnParticle("bouns_hit", GetPositon().x, GetPositon().y);
}