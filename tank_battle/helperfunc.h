#ifndef _HELPERFUNC_H
#define _HELPERFUNC_H

#include <Box2D\Box2D.h>
#include "entity.h"
#include "meshmap.h"

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//一般的坐标转换公式.
//设Oxy, O'x'y'是两个坐标系, O'在Oxy中的坐标为(x0, y0), 由x轴到x'轴的角度为t,坐标转换公式是
//x = x'cost-y'sint + x0, y = x'sint+y'cost + y0.
void CoordinatePointConvert(float& x, float& y, float x0, float y0, float t, float x2, float y2);

//按上下左右分辨entity的方向
enum ENTITY_DIRECTION
{
	ENTITY_DIRECTION_UNDEFINE,
	ENTITY_DIRECTION_UP,
	ENTITY_DIRECTION_LEFT,
	ENTITY_DIRECTION_DOWN,
	ENTITY_DIRECTION_RIGHT
};

ENTITY_DIRECTION GetEntityDirection(float rot);

//ent2在ent1的什么方向
ENTITY_DIRECTION GetDirBetweenEnt(Entity* ent1, Entity* ent2);

//根据方向生成力的b2vec2
b2Vec2 GenVecFromDirection(ENTITY_DIRECTION dir, float v);

//[a,b]范围内的随机整数
int Randint(int a, int b);

//随机方向
ENTITY_DIRECTION RandDir();

//相反方向
ENTITY_DIRECTION GetOppositeDir(ENTITY_DIRECTION dir);

//侧方向
void GetSideDir(std::vector<ENTITY_DIRECTION>& out, ENTITY_DIRECTION dir);

//除某方向外的随机方向
ENTITY_DIRECTION RandDirWithout(ENTITY_DIRECTION without);

//a/b的几率返回true
bool HitChance(int a, int b);

//扩展RECT
RECT ExpandRECT(RECT a, RECT b);

//点是否在rect内
bool IsPointInRect(int x, int y, RECT r);

//2个ent类型判断相关
bool EntTypeIn(char* type1, char* type2, Entity* ent1, Entity* ent2, bool& asc); //ent1=t1 && ent2=t2 || ent1=t2 && ent2=t1
bool EntTypeHas(char* type, Entity* ent1, Entity* ent2, bool& asc); //某一个ent是type
bool EntHas(Entity* ent, Entity* ent1, Entity* ent2, bool asc); //某一个实体是ent

//从Entity生成GeoDesc
GeoDesc GenGeoDesc(Entity* ent);

#endif