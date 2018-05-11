#ifndef _HELPERFUNC_H
#define _HELPERFUNC_H

#include <Box2D\Box2D.h>
#include "entity.h"
#include "meshmap.h"

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//һ�������ת����ʽ.
//��Oxy, O'x'y'����������ϵ, O'��Oxy�е�����Ϊ(x0, y0), ��x�ᵽx'��ĽǶ�Ϊt,����ת����ʽ��
//x = x'cost-y'sint + x0, y = x'sint+y'cost + y0.
void CoordinatePointConvert(float& x, float& y, float x0, float y0, float t, float x2, float y2);

//���������ҷֱ�entity�ķ���
enum ENTITY_DIRECTION
{
	ENTITY_DIRECTION_UNDEFINE,
	ENTITY_DIRECTION_UP,
	ENTITY_DIRECTION_LEFT,
	ENTITY_DIRECTION_DOWN,
	ENTITY_DIRECTION_RIGHT
};

ENTITY_DIRECTION GetEntityDirection(float rot);

//ent2��ent1��ʲô����
ENTITY_DIRECTION GetDirBetweenEnt(Entity* ent1, Entity* ent2);

//���ݷ�����������b2vec2
b2Vec2 GenVecFromDirection(ENTITY_DIRECTION dir, float v);

//[a,b]��Χ�ڵ��������
int Randint(int a, int b);

//�������
ENTITY_DIRECTION RandDir();

//�෴����
ENTITY_DIRECTION GetOppositeDir(ENTITY_DIRECTION dir);

//�෽��
void GetSideDir(std::vector<ENTITY_DIRECTION>& out, ENTITY_DIRECTION dir);

//��ĳ��������������
ENTITY_DIRECTION RandDirWithout(ENTITY_DIRECTION without);

//a/b�ļ��ʷ���true
bool HitChance(int a, int b);

//��չRECT
RECT ExpandRECT(RECT a, RECT b);

//���Ƿ���rect��
bool IsPointInRect(int x, int y, RECT r);

//2��ent�����ж����
bool EntTypeIn(char* type1, char* type2, Entity* ent1, Entity* ent2, bool& asc); //ent1=t1 && ent2=t2 || ent1=t2 && ent2=t1
bool EntTypeHas(char* type, Entity* ent1, Entity* ent2, bool& asc); //ĳһ��ent��type
bool EntHas(Entity* ent, Entity* ent1, Entity* ent2, bool asc); //ĳһ��ʵ����ent

//��Entity����GeoDesc
GeoDesc GenGeoDesc(Entity* ent);

#endif