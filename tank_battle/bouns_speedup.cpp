#include "bouns_speedup.h"

#include "gamestatemanager.h"
#include "gamestate_play.h"
#include "resmanager.h"

Bouns_SpeedUp::Bouns_SpeedUp(float x, float y)
	:Bouns_Base(x, y)
{
	SetTexture(ResManager::GetInstance()->GetTexture("bouns_speedup"));
}

void Bouns_SpeedUp::OnHit(Tank* tank)
{
	GameStateManager* sm = GameStateManager::GetInstance();
	GameState_Play* gsplay = (GameState_Play*)sm->GetCurState();
	int prespeed = tank->GetSpeed();
	if (prespeed > TANK_DEF_SPEED)
	{
		//正在加速中
		return;
	}
	tank->SetSpeed(prespeed + BOUNS_SPEEDUP_VALUE);

	BounsSpeedUpData* passdata = new BounsSpeedUpData;
	passdata->prespeed = prespeed;
	passdata->tank = tank;
	gsplay->AddSecTimerCallBack(FUNC_CALLBACK_2(Bouns_SpeedUp::SpeedUpEnd, this), BOUNS_SPEEDUP_TIME, passdata, true);
}

void Bouns_SpeedUp::SpeedUpEnd(void* data, bool isoncleancall)
{
	if (isoncleancall)
	{
		SAFE_DELETE(data);

		return;
	}

	BounsSpeedUpData bsudata = (*((BounsSpeedUpData*)data));
	if (EntityManager::GetInstance()->IsValidEntity(bsudata.tank))
	{
		bsudata.tank->SetSpeed(bsudata.prespeed);
	}
	SAFE_DELETE(data);
}