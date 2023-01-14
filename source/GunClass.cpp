#include "GunClass.h"

using namespace DirectX::SimpleMath;

GunClass::GunClass():ModelToWorldClass()
{
	SetScale(m_scale);
}

GunClass::~GunClass()
{
}

void GunClass::SetRelativePositionAndRotation(XMFLOAT3 pos, XMFLOAT3 rot)
{
	XMFLOAT3 posOffset;
	float relativeRotationY = (90 - rot.y) * (3.14159265359f / 180.0f);
	float relativeRotationX = (90 - rot.x) * (3.14159265359f / 180.0f);

	posOffset = XMFLOAT3(m_pos.x * cos(relativeRotationY), m_pos.y * cos(relativeRotationX), m_pos.z * sin(relativeRotationY));

	SetPos(pos + posOffset);
	SetRot(rot + m_rot);
}

void GunClass::SetModel(MultiTextureModelClass* newModel)
{
	m_model = newModel;
}

MultiTextureModelClass* GunClass::GetModel()
{
	return m_model;
}
