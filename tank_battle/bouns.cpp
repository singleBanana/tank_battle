#include "bouns.h"
#include "bouns_recover.h"
#include "bouns_power.h"
#include "bouns_pluspoint.h"
#include "bouns_invincible.h"
#include "bouns_speedup.h"
#include "helperfunc.h"

#include <vector>
using namespace std;

Bouns_Base* RandBouns()
{
	Bouns_Base* ret = nullptr;

	vector<BOUNS_TYPE> allbouns;
	allbouns.push_back(BOUNS_TYPE_RECOVER);
	allbouns.push_back(BOUNS_TYPE_POWER);
	allbouns.push_back(BOUNS_TYPE_PLUSPOINT);
	allbouns.push_back(BOUNS_TYPE_INVINCIBLE);
	allbouns.push_back(BOUNS_TYPE_SPEEDUP);
	
	float bx = Randint(0, 800 - BOUNS_WIDTH);
	float by = Randint(0, 600 - BOUNS_HEIGHT);
	switch (allbouns[Randint(0, allbouns.size() - 1)])
	{
		case BOUNS_TYPE_RECOVER:
		{
			ret = new Bouns_Recover(bx, by);
			break;
		}
		case BOUNS_TYPE_POWER:
		{
			ret = new Bouns_Power(bx, by);
			break;
		}
		case BOUNS_TYPE_PLUSPOINT:
		{
			ret = new Bouns_PlusPoint(bx, by);
			break;
		}
		case BOUNS_TYPE_INVINCIBLE:
		{
			ret = new Bouns_Invincible(bx, by);
			break;
		}
		case BOUNS_TYPE_SPEEDUP:
		{
			ret = new Bouns_SpeedUp(bx, by);
			break;
		}
	}

	ret->SetZ(BOUNS_Z);

	return ret;
}