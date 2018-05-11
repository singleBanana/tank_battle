#include "bouns_pluspoint.h"
#include "gamestatemanager.h"
#include "gamestate_play.h"
#include "resmanager.h"

Bouns_PlusPoint::Bouns_PlusPoint(float x, float y)
	:Bouns_Base(x, y)
{
	SetTexture(ResManager::GetInstance()->GetTexture("bouns_pluspoint"));
}

void Bouns_PlusPoint::OnHit(Tank* tank)
{
	GameStateManager* sm = GameStateManager::GetInstance();
	GameState_Play* gsplay = (GameState_Play*)sm->GetCurState();
	if (gsplay->GetPlayer() == tank)
	{
		int p = Randint(10, 100);
		gsplay->SetPoint(gsplay->GetPoint() + p);
	}
}