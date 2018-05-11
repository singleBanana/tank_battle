#include "main.h"
#include "gamestatemanager.h"
#include "gamestate_mainmenu.h"
#include "helpermacro.h"
#include "audiomanager.h"
#include "particlemanager.h"
#include "resmanager.h"

#include <memory>

#include <vld.h> //vld内存泄露检测

std::auto_ptr<MemoryLogger> g_memlogger(MemoryLogger::GetInstance());

std::auto_ptr<GamePlay> g_gameplay(GamePlay::GetInstance());
std::auto_ptr<GameStateManager> g_gsmanager(GameStateManager::GetInstance());
GameState_MainMenu* g_gs_mainmenu = nullptr;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	bool initdone = false;
	do
	{
		//gameplay
		BREAKIF(!g_gameplay->Init(FrameFunc, RenderFunc));
		
		//gamestate
		g_gs_mainmenu = new GameState_MainMenu();
		g_gsmanager->PushState(g_gs_mainmenu);

		initdone = true;

	} while (false);
	
	if (!initdone)
	{
		MessageBoxA(NULL, "Init failed", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	g_gameplay->Run();

	Clean();

	return 0;
}

void Clean()
{
	SAFE_DELETE(g_gs_mainmenu);

	delete AudioManager::GetInstance();
	delete ParticleManager::GetInstance();
//	delete ResManager::GetInstance(); //内存泄露 暂时无法解决
}

bool FrameFunc()
{
	return g_gsmanager->DoThink();
}

bool RenderFunc()
{
	g_gameplay->BeginScene();

	g_gsmanager->DoRender();

	g_gameplay->EndScene();

	return false;
}