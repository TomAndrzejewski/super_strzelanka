#pragma once

#include "EnemyClass.h"

enum GameState
{
	InitialMenu,
	ShootingPhase
};

class GameFlowClass
{
public:
	GameFlowClass();
	~GameFlowClass();

	void StartShootingPhase(vector<EnemyClass>& enemies);
	GameState GetGameState();
	GameState m_GameState;

	
};

