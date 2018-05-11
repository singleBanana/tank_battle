#include "block.h"
#include "particlemanager.h"
#include "resmanager.h"
#include "helpermacro.h"

Block::Block(float x, float y, float width, float height)
	:Entity(x, y, width, height, 0, false, true, false, std::string("block"))
{

}

Block::~Block()
{
	ParticleManager::GetInstance()->SpawnParticle("block_destroy", GetPositon().x, GetPositon().y);
}

bool Block::Hit(int attack)
{
	bool ret = Entity::Hit(attack);

	if (!ret && !IsInvincible())
	{
		ResManager* rm = ResManager::GetInstance();
		float resthppercent = GetHP() * 1.0 / GetMaxHP();
		if (resthppercent < 0.2 || FLOATEQUAL(resthppercent, 0.2))
		{
			SetTexture(rm->GetTexture("block_20"));
		}
		else if (resthppercent < 0.5 || FLOATEQUAL(resthppercent, 0.5))
		{
			SetTexture(rm->GetTexture("block_50"));
		}
	}

	return ret;
}