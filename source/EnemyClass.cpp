#include "EnemyClass.h"


EnemyClass::EnemyClass() : EnemyModelClass(), ModelToWorldClass()
{
	m_EnemyHit = false;
	m_EnemyState = PlayerNotStartedEnum;
}

EnemyClass::~EnemyClass()
{

}


bool EnemyClass::CheckIfEnemyIsHit(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt, float& dist)
{
	return CheckIntersection(cameraPosition, cameraLookAt, dist);
}

bool EnemyClass::CheckIfEnemyIsHit(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt)
{
	return CheckIntersection(cameraPosition, cameraLookAt);
}

void EnemyClass::EnemyGotHit()
{
	if (m_EnemyState == PlayerStartedEnum)
	{
		m_EnemyState = PlayerShootEnemyEnum;
	}
}

void EnemyClass::PlayerStarted()
{
	if (m_EnemyState == PlayerNotStartedEnum)
	{
		m_EnemyState = PlayerStartedEnum;
	}
}

void EnemyClass::PlayerRestarted()
{
	if (m_EnemyState == PlayerShootEnemyEnum)
	{
		m_EnemyState = PlayerNotStartedEnum;
	}
}