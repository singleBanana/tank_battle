#include "entitymanager.h"
#include "helpermacro.h"
#include "helperclass.h"

#include "box2ddef.h"

EntityManager* EntityManager::m_instance = nullptr;

EntityManager::EntityManager()
{
	
}

EntityManager::~EntityManager()
{
	DestroyAllEntity();

	SAFE_DELETE(m_world);
	SAFE_DELETE(m_contactlistener);
	SAFE_DELETE(m_debugdraw);
}

EntityManager* EntityManager::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new EntityManager;
		m_instance->m_world = nullptr;
		m_instance->m_debugdraw = nullptr;
		m_instance->m_drawdebugdata = true;
		m_instance->m_contactlistener = nullptr;
	}

	return m_instance;
}

bool EntityManager::IsValidEntity(Entity* ent)
{
	if (m_allent.count(ent))
	{
		return true;
	}

	return false;
}

void EntityManager::RegisterEntity(Entity* ent)
{
	if (ent)
	{
		m_allent.insert(ent);
	}
}

multiset<int> g_ccc;

void EntityManager::DestroyEntity(Entity* ent)
{
	if (ent)
	{
		m_allent.erase(ent);
		SAFE_DELETE(ent);
	}
}

void EntityManager::DestroyAllEntity()
{
	while (m_allent.size())
	{
		Entity* ent = (*(m_allent.begin()));
		SAFE_DELETE(ent);
		m_allent.erase(m_allent.begin());
	}
}

void EntityManager::SetDebugDraw(bool isdraw)
{
	m_drawdebugdata = isdraw;
}

void EntityManager::InitWorld()
{
	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
	m_world = new b2World(gravity);
	m_world->SetAllowSleeping(false);

	//添加监听
	m_contactlistener = new ContactListener;
	m_world->SetContactListener(m_contactlistener);

	m_debugdraw = new PhysicsDebugDraw;
	uint32 flagsDraw = PhysicsDebugDraw::e_shapeBit;
	m_debugdraw->SetFlags(flagsDraw);
	m_world->SetDebugDraw(m_debugdraw);

//	AddBox(nvissize.x, 1, nvissize.x / 2, 100); //ground
}

b2World* EntityManager::GetWorld()
{
	return m_world;
}

void EntityManager::AddBox(float w, float h, float px, float py)
{
	b2PolygonShape floorShape;

	floorShape.SetAsBox(w / 2 / BOX2D_SCALE_RATIO, h / 2 / BOX2D_SCALE_RATIO);
	b2FixtureDef floorFixture;
	floorFixture.density = 0.0f;
	floorFixture.friction = 10.0f;
	floorFixture.restitution = 0.5f;
	floorFixture.shape = &floorShape;

	b2BodyDef floorBodyDef;
	floorBodyDef.userData = nullptr;
	float bxinp = px + w / 2;
	float byinp = py + h / 2;
	floorBodyDef.position.Set(bxinp / BOX2D_SCALE_RATIO, byinp / BOX2D_SCALE_RATIO);
	b2Body *floorBody = m_world->CreateBody(&floorBodyDef);

	floorBody->CreateFixture(&floorFixture);
}

void EntityManager::Step(float dt, bool drawonly)
{
	//box2d
	//for (b2Body *b = m_world->GetBodyList(); b; b = b->GetNext())
	//{
	//	if (b->GetUserData())
	//	{
	//		Entity *ent = (Entity *)b->GetUserData();
	//		if (ent)
	//		{
	//			b2Vec2 pos = ent->GetPositon();
	//			float rot = ent->GetRotation();

	//			//更新位置
	//			b2Vec2 entsize = ent->GetSize();
	//			float bxinp = pos.x + entsize.x / 2;
	//			float byinp = pos.y + entsize.y / 2;
	//			b2Vec2 b2Position = b2Vec2(bxinp / BOX2D_SCALE_RATIO, byinp / BOX2D_SCALE_RATIO);
	//			b->SetLinearVelocity(b2Vec2(0, 0));
	//			b->SetTransform(b2Position, rot);
	//		}
	//	}
	//}

	for (auto it = m_b2bodytodel.begin(); it != m_b2bodytodel.end(); ++it)
	{
		m_world->DestroyBody(*it);
	}
	m_b2bodytodel.clear();

	if (!drawonly)
	{
		m_world->Step(dt, 10, 10);
	}

	for (b2Body *b = m_world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData())
		{
			Entity *ent = (Entity *)b->GetUserData();
			if (ent)
			{
				//更新位置
				b2Vec2 pos = b->GetPosition();

				b2Vec2 entpos;
				entpos.x = pos.x * BOX2D_SCALE_RATIO;
				entpos.y = pos.y * BOX2D_SCALE_RATIO;
				ent->SetPosition(entpos);

				//hgespr
				hgeSprite* entspr = ent->GetSprite();
				if (entspr)
				{
					entspr->SetZ(ent->GetZ());
					entspr->RenderEx(entpos.x, entpos.y, ent->GetRotation());
				}
			}
		}
	}

	if (m_drawdebugdata)
	{
		m_world->DrawDebugData();
	}
}

void EntityManager::AddContactCallback(b2contactcallback callback)
{
	m_contactlistener->AddContactCallback(callback);
}

void EntityManager::AddB2BodyToDelete(b2Body* b2body)
{
	m_b2bodytodel.insert(b2body);
}