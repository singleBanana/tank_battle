#ifndef _GAMEPLAY_H
#define _GAMEPLAY_H

#include "hge.h"
#include "hgeTileMapXML.h"
#include "hgefont.h"

#include "resmanager.h"

#include <vector>
#include <string>

#define GAMEPLAY_RES_FONTNAME "text"

class GamePlay
{
	struct FontRenderInfo
	{
		std::string str;
		float x;
		float y;
		int align;
	};

	static GamePlay* m_instance;

	HGE* m_hge;
	hgeTileMapXML* m_tilemap;
	hgeFont* m_text;

	std::vector<FontRenderInfo> m_strtorender; //下一桢画出来

	GamePlay();

public:
	~GamePlay();

	static GamePlay* GetInstance();

	HGE* GetHGE();

	bool Init(hgeCallback framefunc, hgeCallback renderfunc);

	void Run();

	void RenderTile(const char* mapname, int x = 0, int y = 0);
	void RenderFPS();
	void RenderCustomString();
	void RenderQuad(hgeQuad* q);

	void PrepareRenderString(const char* str, float x, float y, int align = HGETEXT_LEFT);

	float GetDelta();
	float GetElapsedTime();
	bool GetKeyState(int key);
	bool IsKeyUp(int key);
	bool IsKeyDown(int key);

	void BeginScene();
	void EndScene();

	bool LoadTileMap(const char* filename, const char* mapname);
	tilemapxml::ObjectPtr GetObjectInTile(const char* mapname, char* layername, char* objname);
	bool GetObjectInLayer(const char* mapname, char* layername, std::vector<ObjectPtr>& out);
	MapPtr GetMapPTR(const char* mapname);
	char* GetLayerProperty(const char* mapname, char* layername, char* proname);
};

#endif