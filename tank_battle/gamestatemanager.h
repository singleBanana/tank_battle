#ifndef _GAMESTATEMANAGER_H
#define _GAMESTATEMANAGER_H

#include "gamestate.h"

#include <stack>
using namespace std;

class GameStateManager
{
	static GameStateManager* m_instance;

	stack<GameState*> m_allgs;

	GameStateManager();

public:
	static GameStateManager* GetInstance();

	GameState* GetCurState();
	void ChangeState(GameState* gs);

	void PushState(GameState* gs);
	void PopState();

	bool DoThink();
	void DoRender();
};

#endif