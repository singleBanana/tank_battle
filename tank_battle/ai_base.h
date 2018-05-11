#ifndef _AI_BASE_H
#define _AI_BASE_H

#include "tank.h"
#include "entitymanager.h"

class AI_Base
{
protected:
	Tank* m_tank;

public:
	AI_Base(Tank* tank);
	virtual ~AI_Base();

	Tank* GetTank();

	virtual void Think() = 0;

	virtual void OnContact(Entity* ent);
};

#endif