#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>

#include "hge.h"
#include "hgesprite.h"
#include "Box2D\Box2D.h"

#define ENTITY_RENDER_Z 0.5

class Entity
{
	static int m_entitycount;
	int m_entid;

	std::string m_type;

	//hge
	hgeSprite* m_spr;
	float m_z;

	//box2d
	b2Body* m_b2body;

	//像素为单位
	//size
	float m_width;
	float m_height;
	//pos 中心坐标
	float m_x;
	float m_y;

	float m_rotation; //顺时针旋转的弧度

	//game
	int m_hp;
	int m_maxhp;
	bool m_isinvincible;

	void Destory();

	//box2d
	void SetBox2d(bool isstatic, bool through);

public:
	//x,y为左上角坐标
	Entity(float x, float y, float width, float height, float rot = 0, 
		bool b2donly = false, bool isstatic = true, bool through = false, std::string& type = std::string("entity"));
	virtual ~Entity();

	int GetEntID();

	b2Vec2 GetPositon();
	b2Vec2 GetSize();
	const std::string GetType();
	float GetRotation();
	hgeSprite* GetSprite();
	b2Body* Getb2Body();

	virtual void SetPosition(b2Vec2& pos);
	void SetRotation(float rot);

	//hge
	bool SetTexture(HTEXTURE tex);
	float GetZ();
	void SetZ(float z);

	//box2d
	void SetLinerVelocity(b2Vec2& v);

	//game
	int GetHP();
	void SetHP(int hp);
	int GetMaxHP();
	void SetMaxHP(int maxhp);
	virtual bool Hit(int attack); //hp=0时return true
	bool IsInvincible();
	void SetInvincible(bool invincible);
};

#endif