#pragma once
#include "ModelToWorldClass.h"

#include <d3d11.h>
#include <SimpleMath.h>


class GunClass : public ModelToWorldClass
{
public:
	GunClass();
	~GunClass();

	void SetRelativePositionAndRotation(XMFLOAT3 parentPos,XMFLOAT3 parentRot);

private:
	const XMFLOAT3 m_pos = XMFLOAT3(20.0f, -24.0f, 20.0f);
	const XMFLOAT3 m_rot = XMFLOAT3(0.0f, -15.0f, 0.0f);
	const XMFLOAT3 m_scale = XMFLOAT3(1.0f, 1.0f, -1.0f);
};

