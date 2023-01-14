#include "GameFlowClass.h"

GameFlowClass::GameFlowClass()
{
	m_GameState = GameState::InitialMenu;
}

GameFlowClass::~GameFlowClass()
{
}

void GameFlowClass::StartShootingPhase(vector<EnemyClass>& enemies)
{
	if (m_GameState == GameState::ShootingPhase)
	{
		// Dont do anything because game is already in ShootingPhase
		return;
	}

	for (vector<EnemyClass>::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		it->PlayerStarted();
	}
}

GameState GameFlowClass::GetGameState()
{
	return m_GameState;
}
