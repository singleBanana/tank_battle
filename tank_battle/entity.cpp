#include "entity.h"
#include "entitymanager.h"
#include "box2ddef.h"
#include "helpermacro.h"
#include "helperclass.h"

int Entity::m_entitycount = 0;

Entity::Entity(float x, float y, float width, float height, float rot, 
	bool b2donly, bool isstatic, bool through, std::string& type)
{
	m_z = ENTITY_RENDER_Z;

	++m_entitycount;
	m_entid = m_entitycount;

	m_type = type;

	m_b2body = nullptr;

	m_width = width;
	m_height = height;

	m_x = x + m_width / 2;
	m_y = y + m_height / 2;

	m_rotation = rot;

	if (!b2donly)
	{
		m_spr = new hgeSprite(0, 0, 0, m_width, m_height);
		m_spr->SetHotSpot(m_width / 2, m_height / 2);
	}
	else
	{
		m_spr = nullptr;
	}

	SetBox2d(isstatic, through);

	m_hp = 0;
	m_maxhp = 100;
	m_isinvincible = false;

	EntityManager::GetInstance()->RegisterEntity(this);
}

Entity::~Entity()
{
	Destory();
}

void Entity::Destory()
{
	//hge
	SAFE_DELETE(m_spr);
	
	//box2d
	m_b2body->SetUserData(nullptr);
	EntityManager::GetInstance()->AddB2BodyToDelete(m_b2body);
}

int Entity::GetEntID()
{
	return m_entid;
}

b2Vec2 Entity::GetPositon()
{
	return b2Vec2(m_x, m_y);
}

b2Vec2 Entity::GetSize()
{
	return b2Vec2(m_width, m_height);
}

float Entity::GetRotation()
{
	return m_rotation;
}

const std::string Entity::GetType()
{
	return m_type;
}

hgeSprite* Entity::GetSprite()
{
	return m_spr;
}

b2Body* Entity::Getb2Body()
{
	return m_b2body;
}

void Entity::SetPosition(b2Vec2& pos)
{
	m_x = pos.x;
	m_y = pos.y;

	b2Vec2 b2pos = b2Vec2(m_x / BOX2D_SCALE_RATIO, m_y / BOX2D_SCALE_RATIO);
	m_b2body->SetTransform(b2pos, m_rotation);
}

void Entity::SetRotation(float rot)
{
	m_rotation = rot;
}

bool Entity::SetTexture(HTEXTURE tex)
{
	if (tex)
	{
		m_spr->SetTexture(tex);

		return true;
	}

	return false;
}

float Entity::GetZ()
{
	return m_z;
}

void Entity::SetZ(float z)
{
	m_z = z;
}

void Entity::SetLinerVelocity(b2Vec2& v)
{
	m_b2body->SetLinearVelocity(v);
}

void Entity::SetBox2d(bool isstatic, bool through)
{
	b2World* world = EntityManager::GetInstance()->GetWorld();

	b2PolygonShape b2shape;
	b2shape.SetAsBox(m_width / 2 / BOX2D_SCALE_RATIO, m_height / 2 / BOX2D_SCALE_RATIO);
	b2FixtureDef b2fixture;
	b2fixture.density = isstatic ? 0.0f : 10.0f;
	b2fixture.friction = 0.8f;
	b2fixture.restitution = 0.6f;
	b2fixture.shape = &b2shape;
	b2fixture.isSensor = through ? true : false;

	b2BodyDef bd;
	bd.type = isstatic ? b2_staticBody : b2_dynamicBody;
	bd.userData = this;

	bd.position.Set(m_x / BOX2D_SCALE_RATIO, m_y / BOX2D_SCALE_RATIO);

	m_b2body = world->CreateBody(&bd);
	m_b2body->CreateFixture(&b2fixture);
	m_b2body->SetLinearVelocity(b2Vec2(0, 0));
}

int Entity::GetHP()
{
	return m_hp;
}

void Entity::SetHP(int hp)
{
	int adjhp = hp < 0 ? 0 : hp;
	m_hp = min(adjhp, m_maxhp);
}

int Entity::GetMaxHP()
{
	return m_maxhp;
}

void Entity::SetMaxHP(int maxhp)
{
	m_maxhp = maxhp;
	SetHP(m_hp);
}

bool Entity::Hit(int attack)
{
	if (!IsInvincible())
	{
		int resthp = GetHP() - attack;
		SetHP(resthp);
		if (!GetHP())
		{
			return true;
		}
	}

	return false;
}

bool Entity::IsInvincible()
{
	return m_isinvincible;
}

void Entity::SetInvincible(bool invincible)
{
	m_isinvincible = invincible;
}