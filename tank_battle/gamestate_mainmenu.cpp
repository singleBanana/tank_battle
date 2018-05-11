#include "gamestate_mainmenu.h"
#include "gamestatemanager.h"
#include "helpermacro.h"
#include "menuitem.h"
#include "audiomanager.h"
#include "resmanager.h"

#include <hgeguictrls.h>

GameState_MainMenu::GameState_MainMenu()
{
	ResManager* rm = ResManager::GetInstance();

	hgeSprite* cursor = rm->GetSprite("spr_cursor");
	hgeFont* font = rm->GetFont("menutext");

	//bgquad
	m_bgquad.tex = rm->GetTexture("bg_menu");
	m_bgquad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	for (int i = 0; i<4; i++)
	{
		// Set up z-coordinate of vertices
		m_bgquad.v[i].z = 1.0f;
		// Set up color. The format of DWORD col is 0xAARRGGBB
		m_bgquad.v[i].col = 0xFFFFFFFF;
	}
	m_bgquad.v[0].x = 0; m_bgquad.v[0].y = 0;
	m_bgquad.v[1].x = 800; m_bgquad.v[1].y = 0;
	m_bgquad.v[2].x = 800; m_bgquad.v[2].y = 600;
	m_bgquad.v[3].x = 0; m_bgquad.v[3].y = 600;

	// Create and initialize the GUI
	m_gui = new hgeGUI();

	hgeGUIText* guititle = new hgeGUIText(MAINMENU_ELEMENT_TITLE, 
		340, MAINMENU_TOP_ITEM_Y / 2, 100, MAINMENU_ITEM_HEIGHT, font);
	guititle->SetMode(HGETEXT_LEFT);
	guititle->SetText("Tank Battle");
	m_gui->AddCtrl(guititle);

	HEFFECT menutouchsnd = rm->GetEffect("menu_touch");
	m_gui->AddCtrl(new hgeGUIMenuItem(MAINMENU_ELEMENT_PLAY_SOLO, font, menutouchsnd,
		400, MAINMENU_TOP_ITEM_Y + MAINMENU_ITEM_HEIGHT, 0.0f, "Play Solo"));
	/*m_gui->AddCtrl(new hgeGUIMenuItem(MAINMENU_ELEMENT_PLAY_AI, font, menutouchsnd,
		400, MAINMENU_TOP_ITEM_Y + 2 * MAINMENU_ITEM_HEIGHT, 0.1f, "AI Playing"));*/
	m_gui->AddCtrl(new hgeGUIMenuItem(MAINMENU_ELEMENT_OPTIONS, font, menutouchsnd,
		400, MAINMENU_TOP_ITEM_Y + 2 * MAINMENU_ITEM_HEIGHT, 0.1f, "Options"));
	m_gui->AddCtrl(new hgeGUIMenuItem(MAINMENU_ELEMENT_EXIT, font, menutouchsnd,
		400, MAINMENU_TOP_ITEM_Y + 3 * MAINMENU_ITEM_HEIGHT, 0.2f, "Exit"));

	m_gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
	m_gui->SetCursor(cursor);

	m_bgmeff = rm->GetEffect("bgm_mainmenu");

	m_gsplay = nullptr;
	m_opmenu = nullptr;
}

GameState_MainMenu::~GameState_MainMenu()
{
	SAFE_DELETE(m_gui);
	SAFE_DELETE(m_gsplay);
	SAFE_DELETE(m_opmenu);
}

void GameState_MainMenu::UpdateBGQuad(float dt)
{
	static float t = 0.0f;
	float tx, ty;

	t += dt;
	tx = 50 * cosf(t / 60);
	ty = 50 * sinf(t / 60);

	m_bgquad.v[0].tx = tx;        m_bgquad.v[0].ty = ty;
	m_bgquad.v[1].tx = tx + 800 / 64; m_bgquad.v[1].ty = ty;
	m_bgquad.v[2].tx = tx + 800 / 64; m_bgquad.v[2].ty = ty + 600 / 64;
	m_bgquad.v[3].tx = tx;        m_bgquad.v[3].ty = ty + 600 / 64;
}

void GameState_MainMenu::Render()
{
	// we are using GUI in this state, so it will draw the mouse cursor itself
	// no need to bother
	m_gui->Render();

	GamePlay::GetInstance()->RenderQuad(&m_bgquad);
}

bool GameState_MainMenu::Think()
{
	int id;
	static int lastid = MAINMENU_ELEMENT_NONE_SELECTED;

	if (KEYDOWN(HGEK_ESCAPE))
	{
		lastid = MAINMENU_ELEMENT_EXIT;
		m_gui->Leave();
	}

	GamePlay* gp = GamePlay::GetInstance();
	AudioManager* am = AudioManager::GetInstance();
	float dt = gp->GetDelta();

	// We update the GUI and take an action if
	// one of the menu items was selected
	id = m_gui->Update(dt);
	if (id == -1)
	{
		switch (lastid)
		{
		case MAINMENU_ELEMENT_PLAY_SOLO:
		{
			if (!m_gsplay)
			{
				m_gsplay = new GameState_Play;
				m_gsplay->SetSolo(true);
			}
			am->BGMSlide(0.3f, -2);
			GameStateManager::GetInstance()->PushState(m_gsplay);

			break;
		}
		/*case MAINMENU_ELEMENT_PLAY_AI:
		{
			if (!m_gsplay)
			{
				m_gsplay = new GameState_Play;
				m_gsplay->SetSolo(false);
			}
			am->BGMSlide(0.3f, -2);
			GameStateManager::GetInstance()->PushState(m_gsplay);

			break;
		}*/
		case MAINMENU_ELEMENT_OPTIONS:
		{
			if (!m_opmenu)
			{
				m_opmenu = new GameState_OptionMenu;
			}
			GameStateManager::GetInstance()->PushState(m_opmenu);

			break;
		}
		case MAINMENU_ELEMENT_EXIT:
		{
			return true;
		}
		}
	}
	else if (id)
	{
		lastid = id;
		m_gui->Leave();
	}

	UpdateBGQuad(dt);

	return false;
}

void GameState_MainMenu::OnEnterState()
{
	m_gui->Reset();
	m_gui->SetFocus(MAINMENU_ELEMENT_PLAY_SOLO);
	m_gui->Enter();

	AudioManager::GetInstance()->PlayBGM(m_bgmeff);
}