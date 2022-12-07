////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetFrameTime(float);
	void GetRotation(float&, float&);
	void GetPosition(float&, float&);
	void SetPosition(float, float);
	void SetRotation(float, float);

	void TurnLeft(bool);
	void TurnRight(bool);
	void RotateY(int);
	void RotateX(int);
	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveLeft(bool);
	void MoveRight(bool);

private:
	float m_frameTime;
	float m_rotationY, m_rotationX;
	float m_positionZ, m_positionX;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_forwardMoveSpeed, m_backwardMoveSpeed;
	float m_leftMoveSpeed, m_rightMoveSpeed;
};

#endif