#include "ModelToWorldClass.h"


ModelToWorldClass::ModelToWorldClass()
{
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_PosRotScaleChanged = false;
	m_ModelToWorldMatrix = XMMATRIX(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

ModelToWorldClass::~ModelToWorldClass()
{

}

XMMATRIX ModelToWorldClass::GetModelToWorldMatrix()
{
	// Do not calculate it over and over, only when its needed
	if (m_PosRotScaleChanged)
	{
		m_PosRotScaleChanged = false;

		// Create pos matrix
		XMMATRIX PosMatrix = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

		// Create rot matrix
		float yaw, pitch, roll;
		pitch = m_Rotation.x * 0.0174532925f;
		yaw = m_Rotation.y * 0.0174532925f;
		roll = m_Rotation.z * 0.0174532925f;
		XMMATRIX RotMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

		// Create scale matrix
		XMMATRIX ScaleMatrix = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

		// Combine them
		m_ModelToWorldMatrix = ScaleMatrix * RotMatrix * PosMatrix;
		return m_ModelToWorldMatrix;
	}
	else
	{
		return m_ModelToWorldMatrix;
	}
}