#pragma once

#include "PlayerCollisionClass.h"

class PlayerClass : public PlayerCollisionClass
{
public:
	PlayerClass();
	PlayerClass(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation);
	~PlayerClass();

	void RememberLastPosAndRot(const XMFLOAT3& pos, const XMFLOAT3& rot, bool collisionOccured);
	void GetLastPosAndRot(XMFLOAT3& pos, XMFLOAT3& rot);

	void PlayerKilledEnemy();
	int HowManyEnemiesKilled();
	void RestartKilledEnemiesCounter();

private:
	XMFLOAT3 m_LastPos;
	XMFLOAT3 m_LastRot;

	int m_KilledEnemies;
};

