#include "ContactListener.h"

ContactListener::ContactListener()
{
	
}

void ContactListener::BeginContact(b2Contact* contact)
{
	if (contact)
	{
		Vec2b2fixture contactpair;
		contactpair.f1 = contact->GetFixtureA();
		contactpair.f2 = contact->GetFixtureB();
		m_contactlist.push_back(contactpair);

		for (int i = 0; i < m_contackcallback.size(); ++i)
		{
			m_contackcallback[i](contactpair);
		}
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	if (contact)
	{
		Vec2b2fixture contactpair;
		contactpair.f1 = contact->GetFixtureA();
		contactpair.f2 = contact->GetFixtureB();
		std::list<Vec2b2fixture>::iterator pos;
		pos = std::find(m_contactlist.begin(), m_contactlist.end(), contactpair);
		if (pos != m_contactlist.end())
		{
			m_contactlist.erase(pos);
		}
	}
}

void ContactListener::GetContactList(std::list<Vec2b2fixture>& out)
{
	out = m_contactlist;
}

void ContactListener::AddContactCallback(b2contactcallback callback)
{
	m_contackcallback.push_back(callback);
}