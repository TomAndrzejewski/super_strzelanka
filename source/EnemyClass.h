#pragma once

#include "EnemyModelClass.h"


enum EnemyStateEnum
{
	PlayerNotStartedEnum,	// enemy should not be rendered
	PlayerStartedEnum,		// enemy should be rendered and waiting for player to shoot it down
	PlayerShootEnemyEnum,	// enemy has been hit and should not be rendered, waiting for PlayerStarted state
};


class EnemyClass : public EnemyModelClass
{
public:
	EnemyClass();
	~EnemyClass();

	bool CheckIfEnemyIsHit(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt, float& dist);
	bool CheckIfEnemyIsHit(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt);

	void EnemyGotHit();
	void PlayerStarted();
	void PlayerRestarted();

	EnemyStateEnum GetEnemyState() { return m_EnemyState; }
private:
	bool m_EnemyHit;
	EnemyStateEnum m_EnemyState;
};