#ifndef _GAMESTATE_OPTION_H
#define _GAMESTATE_OPTION_H

#include "gamestate.h"

#include <hgegui.h>
#include <hgeguictrls.h>

class GameState_OptionMenu : public GameState
{
	enum OPTIONMENU_ELEMENT
	{
		OPTIONMENU_ELEMENT_NONE_SELECTED = 0,
		OPTIONMENU_ELEMENT_TITLE = 1,
		OPTIONMENU_ELEMENT_HELP,
		OPTIONMENU_ELEMENT_TEXT_BGM,
		OPTIONMENU_ELEMENT_BACK
	};

protected:
	hgeGUI* m_gui;
	hgeGUIText* m_bgmvtext;
	hgeQuad m_bgquad;

public:
	GameState_OptionMenu();
	~GameState_OptionMenu();

	void UpdateBGQuad(float dt);
	void UpdateBGMVText();

	void Render();
	bool Think();
	void OnEnterState();

	static const int OPTIONMENU_TOP_ITEM_Y = 200;
	static const int OPTIONMENU_ITEM_HEIGHT = 50;
};

#endif