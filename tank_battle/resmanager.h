#ifndef _RESMANAGER_H
#define _RESMANAGER_H

#include "hgeresource.h"

#define GAMEPLAY_RESSCRIPTNAME "res.hgeres"

class ResManager
{
	static ResManager* m_instance;

	hgeResourceManager* m_resmanager;

	ResManager();

public:
	~ResManager();

	static ResManager* GetInstance();

	HTEXTURE GetTexture(const char* name);
	hgeSprite* GetSprite(const char* name);
	hgeFont* GetFont(const char* name);
	HEFFECT GetEffect(const char* name);
	hgeParticleSystem* GetParticleSystem(const char* name);
};

#endif