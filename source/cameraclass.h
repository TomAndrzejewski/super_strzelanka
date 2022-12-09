////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMVECTOR GetPositionVector();
	XMVECTOR GetLookAtVector();
	XMVECTOR GetRotationVector();

	void Render();
	void GetViewMatrix(XMMATRIX&);
	XMMATRIX& GetViewMatrix();

private:
	XMMATRIX m_viewMatrix;
	XMVECTOR m_positionVector, m_lookAtVector;
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
};

#endif