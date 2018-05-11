#include "gameplay.h"
#include "helpermacro.h"

GamePlay* GamePlay::m_instance = nullptr;

GamePlay::GamePlay()
{
	
}

GamePlay::~GamePlay()
{
	SAFE_DELETE(m_tilemap);

	// Restore video mode and free all allocated resources
	m_hge->System_Shutdown();

	// Release the HGE interface. If there are no more references, the HGE object will be deleted.
	m_hge->Release();
}

GamePlay* GamePlay::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new GamePlay;
		m_instance->m_hge = nullptr;
		m_instance->m_tilemap = nullptr;
		m_instance->m_text = nullptr;
	}

	return m_instance;
}

HGE* GamePlay::GetHGE()
{
	return m_hge;
}

bool GamePlay::Init(hgeCallback framefunc, hgeCallback renderfunc)
{
	bool ret = false;
	do
	{
		BREAKIF(!framefunc);
		BREAKIF(!renderfunc);

		m_hge = hgeCreate(HGE_VERSION);
		BREAKIF(!m_hge);

		m_hge->System_SetState(HGE_FRAMEFUNC, framefunc);
		m_hge->System_SetState(HGE_RENDERFUNC, renderfunc);
		m_hge->System_SetState(HGE_SHOWSPLASH, false);

		m_hge->System_SetState(HGE_FPS, 60);

		m_hge->System_SetState(HGE_SCREENWIDTH, 800);
		m_hge->System_SetState(HGE_SCREENHEIGHT, 600);
		m_hge->System_SetState(HGE_SCREENBPP, 32);

		m_hge->System_SetState(HGE_TITLE, "Tank Battle");
		m_hge->System_SetState(HGE_WINDOWED, true);

		m_hge->System_SetState(HGE_ZBUFFER, true);

		m_hge->System_SetState(HGE_USESOUND, true);

		m_hge->System_SetState(HGE_LOGFILE, "log.log");

		if (m_hge->System_Initiate())
		{
			//load font for text
			m_text = ResManager::GetInstance()->GetFont(GAMEPLAY_RES_FONTNAME);
		}
		else
		{
			MessageBoxA(NULL, m_hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);

			break;
		}

		ret = true;
	} while (false);

	return ret;
}

void GamePlay::Run()
{
	m_hge->System_Start();
}

void GamePlay::RenderTile(const char* mapname, int x, int y)
{
	m_tilemap->Render(mapname, x, y);
}

void GamePlay::RenderFPS()
{
	m_text->printf(10, 10, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", m_hge->Timer_GetDelta(), m_hge->Timer_GetFPS());
}

void GamePlay::RenderCustomString()
{
	for (int i = 0; i < m_strtorender.size(); ++i)
	{
		m_text->printf(m_strtorender[i].x, m_strtorender[i].y, m_strtorender[i].align, m_strtorender[i].str.c_str());
	}
	m_strtorender.clear();
}

void GamePlay::RenderQuad(hgeQuad* q)
{
	if (q)
	{
		m_hge->Gfx_RenderQuad(q);
	}
}

void GamePlay::PrepareRenderString(const char* str, float x, float y, int align)
{
	FontRenderInfo fr;
	fr.str = str;
	fr.x = x;
	fr.y = y;
	fr.align = align;

	m_strtorender.push_back(fr);
	
}

float GamePlay::GetDelta()
{
	return m_hge->Timer_GetDelta();
}

float GamePlay::GetElapsedTime()
{
	return m_hge->Timer_GetTime();
}

bool GamePlay::GetKeyState(int key)
{
	return m_hge->Input_GetKeyState(key);
}

bool GamePlay::IsKeyUp(int key)
{
	return m_hge->Input_KeyUp(key);
}

bool GamePlay::IsKeyDown(int key)
{
	return m_hge->Input_KeyDown(key);
}

void GamePlay::BeginScene()
{
	m_hge->Gfx_BeginScene();
	m_hge->Gfx_Clear(0);
}

void GamePlay::EndScene()
{
	m_hge->Gfx_EndScene();
}

bool GamePlay::LoadTileMap(const char* filename, const char* mapname)
{
	bool ret = false;
	do
	{
		BREAKIF(!filename);
		BREAKIF(!mapname);

		SAFE_DELETE(m_tilemap);

		m_tilemap = new hgeTileMapXML(m_hge);
		char* tmxpath = m_hge->Resource_MakePath(filename);
		BREAKIF(!tmxpath);
		m_tilemap->LoadMap(mapname, tmxpath);

		ret = true;

	} while (false);

	return ret;
}

tilemapxml::ObjectPtr GamePlay::GetObjectInTile(const char* mapname, char* layername, char* objname)
{
	ObjectPtr ret = nullptr;
	do
	{
		MapPtr map = m_tilemap->GetMap(mapname);
		BREAKIF(!map);
		Layer* layer = map->GetLayer(layername);
		BREAKIF(!layer);
		ret = layer->GetObjectFromName(objname);

	} while (false);

	return ret;
}

bool GamePlay::GetObjectInLayer(const char* mapname, char* layername, std::vector<ObjectPtr>& out)
{
	bool ret = false;
	do
	{
		MapPtr map = m_tilemap->GetMap(mapname);
		BREAKIF(!map);
		Layer* layer = map->GetLayer(layername);
		BREAKIF(!layer);
		layer->GetAllObject(out);

		ret = true;

	} while (false);

	return ret;
}

MapPtr GamePlay::GetMapPTR(const char* mapname)
{
	return m_tilemap->GetMap(mapname);
}

char* GamePlay::GetLayerProperty(const char* mapname, char* layername, char* proname)
{
	char* ret = nullptr;
	do
	{
		MapPtr map = m_tilemap->GetMap(mapname);
		BREAKIF(!map);
		Layer* layer = map->GetLayer(layername);
		BREAKIF(!layer);
		PropertyPtr prop = layer->GetProperty(proname);
		BREAKIF(!prop);
		ret = prop->GetValue();

	} while (false);

	return ret;
}