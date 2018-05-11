#include "bouns_power.h"

#include "resmanager.h"

Bouns_Power::Bouns_Power(float x, float y)
	:Bouns_Base(x, y)
{
	SetTexture(ResManager::GetInstance()->GetTexture("bouns_power"));
}

void Bouns_Power::OnHit(Tank* tank)
{
	tank->SetBulletPower(tank->GetBulletPower() + BOUNS_POWER_VALUE);
}