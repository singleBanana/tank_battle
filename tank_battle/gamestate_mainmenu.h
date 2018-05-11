#ifndef _GAMESTATE_MAINMENU_H
#define _GAMESTATE_MAINMENU_H

#include "gamestate.h"
#include "gamestate_play.h"
#include "gamestate_option.h"

#include <hgegui.h>

class GameState_MainMenu : public GameState
{
	enum MAINMENU_ELEMENT
	{
		MAINMENU_ELEMENT_NONE_SELECTED = 0,
		MAINMENU_ELEMENT_TITLE = 1,
		MAINMENU_ELEMENT_PLAY_SOLO,
	//	MAINMENU_ELEMENT_PLAY_AI,
		MAINMENU_ELEMENT_OPTIONS,
		MAINMENU_ELEMENT_EXIT
	};

protected:
	hgeGUI* m_gui;
	hgeQuad m_bgquad;

	HEFFECT m_bgmeff;

	GameState_Play* m_gsplay;
	GameState_OptionMenu* m_opmenu;

public:
	GameState_MainMenu();
	~GameState_MainMenu();

	void UpdateBGQuad(float dt);

	void Render();
	bool Think();
	void OnEnterState();

	static const int MAINMENU_TOP_ITEM_Y = 200;
	static const int MAINMENU_ITEM_HEIGHT = 40;
};


#endif