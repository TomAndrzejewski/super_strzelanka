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
	float posOffset;
	float relativeRotationX = (90 - rot.x) * (3.14159265359f / 180.0f);

	posOffset = m_pos.y * cos(relativeRotationX);


	float roationRad = (rot.y) * (3.14159265359f / 180.0f);
	pos.z -= m_pos.x * sin(roationRad);
	pos.x += m_pos.x * cos(roationRad);

	roationRad = (90 - rot.y) * (3.14159265359f / 180.0f);

	pos.z += m_pos.z * sin(roationRad);
	pos.x += m_pos.z * cos(roationRad);

	pos.y += posOffset;

	SetPos(pos);
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
