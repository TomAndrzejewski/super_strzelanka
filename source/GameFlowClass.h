#pragma once

#include "EnemyClass.h"

enum class GameState
{
	InitialMenu,
	ShootingPhase
};

class GameFlowClass
{
	GameFlowClass();
	~GameFlowClass();

	void StartShootingPhase(vector<EnemyClass>& enemies);

private:
	GameState m_GameState;
};

