#include "gamestate_option.h"
#include "gamestatemanager.h"
#include "helpermacro.h"
#include "audiomanager.h"
#include "resmanager.h"

GameState_OptionMenu::GameState_OptionMenu()
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

	hgeGUIText* guititle = new hgeGUIText(OPTIONMENU_ELEMENT_TITLE,
		340, OPTIONMENU_TOP_ITEM_Y / 2, 100, OPTIONMENU_ITEM_HEIGHT, font);
	guititle->SetMode(HGETEXT_LEFT);
	guititle->SetText("Option");
	m_gui->AddCtrl(guititle);

	hgeGUIText* guihelp = new hgeGUIText(OPTIONMENU_ELEMENT_HELP,
		340, OPTIONMENU_TOP_ITEM_Y, 100, OPTIONMENU_ITEM_HEIGHT, font);
	guihelp->SetMode(HGETEXT_CENTER);
	guihelp->SetText("Use arrow key up and down to adjust bgm volumn\nPress esc to back");
	m_gui->AddCtrl(guihelp);

	m_bgmvtext = new hgeGUIText(OPTIONMENU_ELEMENT_TEXT_BGM,
		170, OPTIONMENU_TOP_ITEM_Y * 1.3, 100, OPTIONMENU_ITEM_HEIGHT, font);
	m_bgmvtext->SetMode(HGETEXT_LEFT);
	UpdateBGMVText();
	m_gui->AddCtrl(m_bgmvtext);

	m_gui->SetNavMode(HGEGUI_NONAVKEYS);
	m_gui->SetCursor(cursor);
}

GameState_OptionMenu::~GameState_OptionMenu()
{
	SAFE_DELETE(m_gui);
}

void GameState_OptionMenu::UpdateBGQuad(float dt)
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

void GameState_OptionMenu::UpdateBGMVText()
{
	AudioManager* am = AudioManager::GetInstance();
	char temp[8];
	string bgmvstr;
	bgmvstr += "bgm volumn(0-100): ";
	bgmvstr += _itoa(am->GetBGMVolumn(), temp, 10);
	m_bgmvtext->SetText(bgmvstr.c_str());
}

void GameState_OptionMenu::Render()
{
	// we are using GUI in this state, so it will draw the mouse cursor itself
	// no need to bother
	m_gui->Render();

	GamePlay::GetInstance()->RenderQuad(&m_bgquad);
}

bool GameState_OptionMenu::Think()
{
	int id;
	static int lastid = OPTIONMENU_ELEMENT_NONE_SELECTED;

	GamePlay* gp = GamePlay::GetInstance();
	float dt = gp->GetDelta();
	AudioManager* am = AudioManager::GetInstance();

	if (KEYDOWN(HGEK_UP))
	{
		am->SetBGMVolumn(am->GetBGMVolumn() + 2);
		UpdateBGMVText();
	}
	if (KEYDOWN(HGEK_DOWN))
	{
		am->SetBGMVolumn(am->GetBGMVolumn() - 2);
		UpdateBGMVText();
	}

	if (KEYDOWN(HGEK_ESCAPE))
	{
		lastid = OPTIONMENU_ELEMENT_BACK;
		m_gui->Leave();
	}

	// We update the GUI and take an action if
	// one of the menu items was selected
	id = m_gui->Update(dt);
	if (id == -1)
	{
		switch (lastid)
		{
		case OPTIONMENU_ELEMENT_BACK:
		{
			GameStateManager::GetInstance()->PopState();

			break;
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

void GameState_OptionMenu::OnEnterState()
{
	m_gui->Reset();
	m_gui->Enter();
}