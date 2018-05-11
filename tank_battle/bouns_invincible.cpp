#include "bouns_invincible.h"
#include "gamestatemanager.h"
#include "gamestate_play.h"
#include "resmanager.h"
#include "particlemanager.h"

Bouns_Invincible::Bouns_Invincible(float x, float y)
	:Bouns_Base(x, y)
{
	SetTexture(ResManager::GetInstance()->GetTexture("bouns_invincible"));
}

void Bouns_Invincible::OnHit(Tank* tank)
{
	GameStateManager* sm = GameStateManager::GetInstance();
	GameState_Play* gsplay = (GameState_Play*)sm->GetCurState();
	tank->SetInvincible(true);
	ParticleManager::GetInstance()->SpawnParticleFollowEntity("bouns_invincible", tank, false);
	gsplay->AddSecTimerCallBack(FUNC_CALLBACK_2(Bouns_Invincible::InvincibleEnd, this), BOUNS_INVINCIBLE_TIME, tank);
}

void Bouns_Invincible::InvincibleEnd(void* tank, bool isoncleancall)
{
	if (EntityManager::GetInstance()->IsValidEntity((Tank*)tank))
	{
		((Tank*)tank)->SetInvincible(false);
		ParticleManager::GetInstance()->KillParticleFromFollowedEnt((Tank*)tank);
	}
}