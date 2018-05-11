#ifndef _CONTACTLISTENER_H
#define _CONTACTLISTENER_H

#include <Box2D\Box2D.h>

#include <vector>
#include <list>
#include <functional>

#include "box2ddef.h"

typedef std::function<void(Vec2b2fixture)> b2contactcallback;

class ContactListener : public b2ContactListener
{
	std::list<Vec2b2fixture> m_contactlist;
	std::vector<b2contactcallback> m_contackcallback;

	//Åö×²¿ªÊ¼
	void BeginContact(b2Contact* contact);
	//Åö×²½áÊø
	void EndContact(b2Contact* contact);

public:
	ContactListener();

	void GetContactList(std::list<Vec2b2fixture>& out);
	void AddContactCallback(b2contactcallback callback);
};

#endif