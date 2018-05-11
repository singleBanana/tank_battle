#include "bouns_recover.h"

#include "resmanager.h"

Bouns_Recover::Bouns_Recover(float x, float y)
	:Bouns_Base(x, y)
{
	SetTexture(ResManager::GetInstance()->GetTexture("bouns_recover"));
}

void Bouns_Recover::OnHit(Tank* tank)
{
	tank->Hit(-BOUNS_RECOVER_VALUE);
}