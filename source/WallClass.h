#pragma once

#include "SimpleModelClass.h"
#include "ModelToWorldClass.h"
#include "BBoxCollisionClass.h"


class WallClass : public ModelToWorldClass
{
public:
	WallClass();
	WallClass(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation);
	~WallClass();

	void SetCollisionBBox(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation);
	void SetCollisionBBox(const ModelOrganizerStruct& wall);

	void SetColor(XMFLOAT4 color) { m_color = color; }
	XMFLOAT4 GetColor() { return m_color; }

	SimpleModelClass* GetModel() { return m_model; }
	bool SetModel(SimpleModelClass* model) { if (model == nullptr) return false;	else { m_model = model; return true; } }

	BBoxCollisionClass& GetBBox() { return m_BBox; }

private:
	SimpleModelClass* m_model;
	BBoxCollisionClass m_BBox;
	XMFLOAT4 m_color;
};