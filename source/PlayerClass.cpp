#include "PlayerClass.h"

PlayerClass::PlayerClass() : PlayerCollisionClass()
{
	m_LastPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_LastRot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_KilledEnemies = 0;
}

PlayerClass::PlayerClass(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation) : PlayerCollisionClass(position, extends, orientation)
{
	m_LastPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_LastRot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_KilledEnemies = 0;
}

PlayerClass::~PlayerClass()
{

}

void PlayerClass::RememberLastPosAndRot(const XMFLOAT3& pos, const XMFLOAT3& rot, bool collisionOccured)
{
	//m_LastPos = pos;
	//m_LastRot = rot;
	// Local buffor for this method
	static XMFLOAT3 posFiveFramesBack[6];
	static XMFLOAT3 rotFiveFramesBack[6];
	static int readIndex = 0, writeIndex = 0;
	static int counterFromLastCollision = 0;

	// Here simple circullar buffer with additional delay of five records is used
	if (!collisionOccured)
	{
		if (counterFromLastCollision == 1) // read always reads with five records delay
		{
			posFiveFramesBack[writeIndex] = pos;
			//rotFiveFramesBack[writeIndex] = rot;

			m_LastPos = posFiveFramesBack[readIndex];
			m_LastRot = rotFiveFramesBack[readIndex];

			char str[500];
			sprintf_s(str, "posFiveFramesBack[readIndex]: %f %f %f\n rotFiveFramesBack[readIndex]: %f %f %f\n", posFiveFramesBack[readIndex].x, posFiveFramesBack[readIndex].y, posFiveFramesBack[readIndex].z, rotFiveFramesBack[readIndex].x, rotFiveFramesBack[readIndex].y, rotFiveFramesBack[readIndex].z);
			OutputDebugStringA(str);

			if (writeIndex == 1) { writeIndex = 0; }
			else { writeIndex++; }

			if (readIndex == 0) { readIndex = 0; }
			else { readIndex++; }
		}
		else // only write, delay is not 5
		{
			counterFromLastCollision++;
			char str[500];
			sprintf_s(str, "counterFromLastCollision: %d\n", counterFromLastCollision);
			OutputDebugStringA(str);

			posFiveFramesBack[writeIndex] = pos;
			//rotFiveFramesBack[writeIndex] = rot;

			sprintf_s(str, "posFiveFramesBack[writeIndex]: %f %f %f\n", posFiveFramesBack[writeIndex].x, posFiveFramesBack[writeIndex].y, posFiveFramesBack[writeIndex].z);
			OutputDebugStringA(str);

			if (writeIndex == 1) { writeIndex = 0; }
			else { writeIndex++; }
		}
	}
	else // reset whole buffer and delay
	{
		counterFromLastCollision = 0;
		writeIndex = 0;
		readIndex = 0;
	}

	m_LastRot = rot;
}

void PlayerClass::GetLastPosAndRot(XMFLOAT3& pos, XMFLOAT3& rot)
{
	pos = m_LastPos;
	rot = m_LastRot;
}

void PlayerClass::PlayerKilledEnemy()
{
	m_KilledEnemies++;
}

int PlayerClass::HowManyEnemiesKilled()
{
	return m_KilledEnemies;
}

void PlayerClass::RestartKilledEnemiesCounter()
{
	m_KilledEnemies = 0;
}