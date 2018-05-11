#include "helperfunc.h"
#include "helpermacro.h"
#include "constdef.h"

#include <math.h>
#include <time.h>
#include <vector>

void CoordinatePointConvert(float& x, float& y, float x0, float y0, float t, float x2, float y2)
{
	x = x2 * cos(t) - y2 * sin(t) + x0;
	y = x2 * sin(t) + y2 * cos(t) + y0;
}

ENTITY_DIRECTION GetEntityDirection(float rot)
{
	ENTITY_DIRECTION ret = ENTITY_DIRECTION_UNDEFINE;
	if (FLOATEQUAL(rot, 0)) //上
	{
		ret = ENTITY_DIRECTION_UP;
	}
	else if (FLOATEQUAL(rot, MATH_PI)) //下
	{
		ret = ENTITY_DIRECTION_DOWN;
	}
	else if (FLOATEQUAL(rot, MATH_PI * 3 / 2)) //左
	{
		ret = ENTITY_DIRECTION_LEFT;
	}
	else if (FLOATEQUAL(rot, MATH_PI / 2)) //右
	{
		ret = ENTITY_DIRECTION_RIGHT;
	}

	return ret;
}

ENTITY_DIRECTION GetDirBetweenEnt(Entity* ent1, Entity* ent2)
{
	b2Vec2 pos1 = ent1->GetPositon();
	b2Vec2 pos2 = ent2->GetPositon();
	float dx = fabs(pos1.x - pos2.x);
	float dy = fabs(pos1.y - pos2.y);
	if (dx > dy) //左右
	{
		if (pos1.x < pos2.x)
		{
			return ENTITY_DIRECTION_RIGHT;
		}
		else
		{
			return ENTITY_DIRECTION_LEFT;
		}
	}
	else //上下
	{
		if (pos1.y < pos2.y)
		{
			return ENTITY_DIRECTION_DOWN;
		}
		else
		{
			return ENTITY_DIRECTION_UP;
		}
	}
}

b2Vec2 GenVecFromDirection(ENTITY_DIRECTION dir, float v)
{
	b2Vec2 ret;
	switch (dir)
	{
		case ENTITY_DIRECTION_UP:
		{
			ret.x = 0;
			ret.y = -v;

			break;
		}
		case ENTITY_DIRECTION_DOWN:
		{
			ret.x = 0;
			ret.y = v;

			break;
		}
		case ENTITY_DIRECTION_LEFT:
		{
			ret.x = -v;
			ret.y = 0;

			break;
		}
		case ENTITY_DIRECTION_RIGHT:
		{
			ret.x = v;
			ret.y = 0;

			break;
		}
	}

	return ret;
}

int Randint(int a, int b)
{
	static bool seedset = false;
	if (!seedset)
	{
		srand(time(nullptr));
		seedset = true;
	}

	return (a + rand() % (b - a + 1));
}

ENTITY_DIRECTION RandDir()
{
	std::vector<ENTITY_DIRECTION> alldir;
	alldir.push_back(ENTITY_DIRECTION_UP);
	alldir.push_back(ENTITY_DIRECTION_LEFT);
	alldir.push_back(ENTITY_DIRECTION_DOWN);
	alldir.push_back(ENTITY_DIRECTION_RIGHT);

	return alldir[Randint(0, 3)];
}

ENTITY_DIRECTION GetOppositeDir(ENTITY_DIRECTION dir)
{
	switch (dir)
	{
		case ENTITY_DIRECTION_UP:
		{
			return ENTITY_DIRECTION_DOWN;
		}
		case ENTITY_DIRECTION_LEFT:
		{
			return ENTITY_DIRECTION_RIGHT;
		}
		case ENTITY_DIRECTION_DOWN:
		{
			return ENTITY_DIRECTION_UP;
		}
		case ENTITY_DIRECTION_RIGHT:
		{
			return ENTITY_DIRECTION_LEFT;
		}
	}

	return ENTITY_DIRECTION_UNDEFINE;
}

void GetSideDir(std::vector<ENTITY_DIRECTION>& out, ENTITY_DIRECTION dir)
{
	out.clear();

	if (dir == ENTITY_DIRECTION_UP || dir == ENTITY_DIRECTION_DOWN)
	{
		out.push_back(ENTITY_DIRECTION_LEFT);
		out.push_back(ENTITY_DIRECTION_RIGHT);
	}
	else
	{
		out.push_back(ENTITY_DIRECTION_UP);
		out.push_back(ENTITY_DIRECTION_DOWN);
	}
}

ENTITY_DIRECTION RandDirWithout(ENTITY_DIRECTION without)
{
	std::vector<ENTITY_DIRECTION> alldir;
	alldir.push_back(ENTITY_DIRECTION_UP);
	alldir.push_back(ENTITY_DIRECTION_LEFT);
	alldir.push_back(ENTITY_DIRECTION_DOWN);
	alldir.push_back(ENTITY_DIRECTION_RIGHT);

	auto itfind = std::find(alldir.begin(), alldir.end(), without);
	if (itfind != alldir.end())
	{
		alldir.erase(itfind);
	}
	
	return alldir[Randint(0, 2)];
}

bool HitChance(int a, int b)
{
	int randint = Randint(1, b);
	if (randint <= a)
	{
		return true;
	}

	return false;
}

RECT ExpandRECT(RECT a, RECT b)
{
	RECT r;
	r.top = min(a.top, b.top);
	r.left = min(a.left, b.left);
	r.right = max(a.right, b.right);
	r.bottom = max(a.bottom, b.bottom);
	
	return r;
}

bool IsPointInRect(int x, int y, RECT r)
{
	if (x >= r.left && x <= r.right &&
		y >= r.top && y <= r.bottom)
	{
		return true;
	}

	return false;
}

bool EntTypeIn(char* type1, char* type2, Entity* ent1, Entity* ent2, bool& asc)
{
	EntityManager* em = EntityManager::GetInstance();
	if (!em->IsValidEntity(ent1) || !em->IsValidEntity(ent2))
	{
		return false;
	}

	std::string t1 = type1;
	std::string t2 = type2;
	if ((ent1 && ent1->GetType() == t1) && (ent2 && ent2->GetType() == t2))
	{
		asc = true;

		return true;
	}
	if ((ent1 && ent1->GetType() == t2) && (ent2 && ent2->GetType() == t1))
	{
		asc = false;

		return true;
	}

	return false;
}

bool EntTypeHas(char* type, Entity* ent1, Entity* ent2, bool& asc)
{
	EntityManager* em = EntityManager::GetInstance();
	if (!em->IsValidEntity(ent1) || !em->IsValidEntity(ent2))
	{
		return false;
	}

	std::string t = type;
	if (ent1 && ent1->GetType() == t)
	{
		asc = true;

		return true;
	}
	if (ent2 && ent2->GetType() == t)
	{
		asc = false;

		return true;
	}

	return false;
}

bool EntHas(Entity* ent, Entity* ent1, Entity* ent2, bool asc)
{
	EntityManager* em = EntityManager::GetInstance();
	if (!em->IsValidEntity(ent1) || !em->IsValidEntity(ent2))
	{
		return false;
	}

	if (ent1 == ent)
	{
		asc = true;

		return true;
	}
	if (ent2 == ent)
	{
		asc = false;

		return true;
	}

	return false;
}

GeoDesc GenGeoDesc(Entity* ent)
{
	GeoDesc geo;
	b2Vec2 pos = ent->GetPositon();
	b2Vec2 entsize = ent->GetSize();
	geo.x = pos.x - entsize.x / 2;
	geo.y = pos.y - entsize.y / 2;;
	geo.width = entsize.x;
	geo.height = entsize.y;

	return geo;
}