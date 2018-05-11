#include "gamestatemanager.h"

GameStateManager* GameStateManager::m_instance = nullptr;

GameStateManager::GameStateManager()
{
	
}

GameStateManager* GameStateManager::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new GameStateManager();
	}

	return m_instance;
}

GameState* GameStateManager::GetCurState()
{
	return m_allgs.top();
}

void GameStateManager::ChangeState(GameState* gs)
{
	m_allgs.pop();
	m_allgs.push(gs);
	gs->OnEnterState();
}

void GameStateManager::PushState(GameState* gs)
{
	m_allgs.push(gs);
	gs->OnEnterState();
}

void GameStateManager::PopState()
{
	m_allgs.pop();
	if (!m_allgs.empty())
	{
		return m_allgs.top()->OnEnterState();
	}
}

bool GameStateManager::DoThink()
{
	if (!m_allgs.empty())
	{
		return m_allgs.top()->Think();
	}

	return true; //think½áÊø
}

void GameStateManager::DoRender()
{
	if (!m_allgs.empty())
	{
		m_allgs.top()->Render();
	}
}