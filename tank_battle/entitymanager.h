#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include "entity.h"
#include "ContactListener.h"
#include "PhysicsDebugDraw.h"

#include "Box2D\Box2D.h"
#include <list>
#include <set>

class EntityManager
{
	static EntityManager* m_instance;

	std::set<Entity*> m_allent;

	b2World* m_world;
	PhysicsDebugDraw* m_debugdraw;
	bool m_drawdebugdata;
	ContactListener* m_contactlistener;
	std::set<b2Body*> m_b2bodytodel;

	EntityManager();

public:
	~EntityManager();

	static EntityManager* GetInstance();

	bool IsValidEntity(Entity* ent);
	void RegisterEntity(Entity* ent);
	void DestroyEntity(Entity* ent);
	void DestroyAllEntity();

	void SetDebugDraw(bool isdraw);

	void InitWorld();
	b2World* GetWorld();
	void AddBox(float w, float h, float px, float py);
	void Step(float dt, bool drawonly = false);
	void AddContactCallback(b2contactcallback callback);

	void AddB2BodyToDelete(b2Body* b2body);
};

#endif