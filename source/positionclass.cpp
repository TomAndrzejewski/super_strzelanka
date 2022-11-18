////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "positionclass.h"

PositionClass::PositionClass()
{
	m_frameTime = 0.0f;
	m_rotationY = 0.0f;
	m_rotationX = 0.0f;
	m_leftTurnSpeed  = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_forwardMoveSpeed = 0.0f;
	m_backwardMoveSpeed = 0.0f;
	m_positionZ = -10.0f;
	m_positionX = 0.0f;
	m_leftMoveSpeed = 0.0f;
	m_rightMoveSpeed = 0.0f;

}


PositionClass::PositionClass(const PositionClass& other)
{
}


PositionClass::~PositionClass()
{
}


void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}


void PositionClass::GetRotation(float& y, float& x)
{
	y = m_rotationY;
	x = m_rotationX;
	return;
}

void PositionClass::RotateY(int xDelta)
{
	m_rotationY += xDelta * m_frameTime * 0.01f;

	if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}

	if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

}

void PositionClass::RotateX(int yDelta)
{
	m_rotationX += yDelta * m_frameTime * 0.01f;

	if (m_rotationX < 0.0f)
	{
		m_rotationX += 360.0f;
	}

	if (m_rotationX > 360.0f)
	{
		m_rotationX -= 360.0f;
	}

}

void PositionClass::TurnLeft(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns left.  If not slow down the turn speed.
	if(keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if(m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime* 0.005f;

		if(m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationY -= m_leftTurnSpeed;
	if(m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}

	return;
}


void PositionClass::TurnRight(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns right.  If not slow down the turn speed.
	if(keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if(m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime* 0.005f;

		if(m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationY += m_rightTurnSpeed;
	if(m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

	return;
}

void PositionClass::MoveForward(bool keydown)
{
	if (keydown)
	{
		m_forwardMoveSpeed += m_frameTime * 0.01f;

		if (m_forwardMoveSpeed > (m_frameTime * 0.10f))
		{
			m_forwardMoveSpeed = m_frameTime * 0.10f;
		}
		
		// takie proste a tyle czasu zjadlo :(((
		float relativeRotation = (90 - m_rotationY) * (3.14159265359f / 180.0f);
		m_positionZ += m_forwardMoveSpeed * sin(relativeRotation);
		m_positionX += m_forwardMoveSpeed * cos(relativeRotation);


	}
	else
	{
		m_forwardMoveSpeed -= m_frameTime * 0.005f;

		if (m_forwardMoveSpeed < 0.0f)
		{
			m_forwardMoveSpeed = 0.0f;
		}
	}

	

	return;
}

void PositionClass::MoveBackward(bool keydown)
{
	if (keydown)
	{
		m_backwardMoveSpeed += m_frameTime * 0.01f;

		if (m_backwardMoveSpeed > (m_frameTime * 0.10f))
		{
			m_backwardMoveSpeed = m_frameTime * 0.10f;
		}

		float relativeRotation = (90 - m_rotationY) * (3.14159265359f / 180.0f);
		m_positionZ -= m_backwardMoveSpeed * sin(relativeRotation);
		m_positionX -= m_backwardMoveSpeed * cos(relativeRotation);

	}
	else
	{
		m_backwardMoveSpeed -= m_frameTime * 0.005f;

		if (m_backwardMoveSpeed < 0.0f)
		{
			m_backwardMoveSpeed = 0.0f;
		}
	}

	

	return;
}

void PositionClass::MoveLeft(bool keydown)
{

	if (keydown)
	{
		m_leftMoveSpeed += m_frameTime * 0.01f;

		if (m_leftMoveSpeed > (m_frameTime * 0.10f))
		{
			m_leftMoveSpeed = m_frameTime * 0.10f;
		}

		float roationRad = (m_rotationY) * (3.14159265359f / 180.0f);
		m_positionZ += m_leftMoveSpeed * sin(roationRad);
		m_positionX -= m_leftMoveSpeed * cos(roationRad);

	}
	else
	{
		m_leftMoveSpeed -= m_frameTime * 0.005f;

		if (m_leftMoveSpeed < 0.0f)
		{
			m_leftMoveSpeed = 0.0f;
		}
	}

	return;
	
}

void PositionClass::MoveRight(bool keydown)
{
	if (keydown)
	{
		m_rightMoveSpeed += m_frameTime * 0.01f;

		if (m_rightMoveSpeed > (m_frameTime * 0.10f))
		{
			m_rightMoveSpeed = m_frameTime * 0.10f;
		}

		float roationRad = (m_rotationY) * (3.14159265359f / 180.0f);
		m_positionZ -= m_rightMoveSpeed * sin(roationRad);
		m_positionX += m_rightMoveSpeed * cos(roationRad);

	}
	else
	{
		m_rightMoveSpeed -= m_frameTime * 0.005f;

		if (m_rightMoveSpeed < 0.0f)
		{
			m_rightMoveSpeed = 0.0f;
		}
	}

	return;
}

void PositionClass::GetPosition(float& z, float& x)
{
	z = m_positionZ;
	x = m_positionX;
}