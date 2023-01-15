#pragma once
#include "ModelToWorldClass.h"

#include <d3d11.h>
#include <SimpleMath.h>
#include "MultiTextureModelClass.h"


class GunClass : public ModelToWorldClass
{
public:
	GunClass();
	~GunClass();

	void SetRelativePositionAndRotation(XMFLOAT3 parentPos,XMFLOAT3 parentRot);
	void SetModel(MultiTextureModelClass*);
	MultiTextureModelClass* GetModel();

private:
	const XMFLOAT3 m_pos = XMFLOAT3(40.0f, -44.0f, 40.0f);
	const XMFLOAT3 m_rot = XMFLOAT3(0.0f, -15.0f, 0.0f);
	const XMFLOAT3 m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	MultiTextureModelClass* m_model;
};

