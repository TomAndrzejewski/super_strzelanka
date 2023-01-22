#pragma once

#include "CommonConsts.h"
#include "CommonTypes.h"

#include <d3d11.h>
#include <directxmath.h>
#include <SimpleMath.h>
using namespace DirectX;

class ModelToWorldClass
{
public:
	ModelToWorldClass();
	~ModelToWorldClass();

	void SetPosAndRotAndScale(const XMFLOAT3& pos, const XMFLOAT3& rot, const XMFLOAT3& scale) { m_Position = pos; m_Rotation = rot; m_Scale = scale; m_PosRotScaleChanged = true; }
	void SetPosAndRotAndScale(const ModelOrganizerStruct& modelOrganizer) { m_Position = modelOrganizer.pos; m_Rotation = modelOrganizer.rot; m_Scale = modelOrganizer.scale; m_PosRotScaleChanged = true; }

	void SetPos(const XMFLOAT3& pos) { m_Position = pos; m_PosRotScaleChanged = true; }
	void SetRot(const XMFLOAT3& rot) { m_Rotation = rot; m_PosRotScaleChanged = true; }
	void SetScale(const XMFLOAT3& scale) { m_Scale = scale; m_PosRotScaleChanged = true; }

	XMMATRIX GetModelToWorldMatrix();

private:
	XMFLOAT3 m_Position, m_Rotation, m_Scale;
	XMMATRIX m_ModelToWorldMatrix;
	bool m_PosRotScaleChanged;
};

