#include "WallClass.h"


WallClass::WallClass() : ModelToWorldClass()
{
	m_model = nullptr;
	m_color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

WallClass::WallClass(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation)
{
	SetCollisionBBox(position, extends, orientation);
}

WallClass::~WallClass()
{

}

void WallClass::SetCollisionBBox(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation)
{
	m_BBox.SetBoundingBox(position, extends, orientation);
}

void WallClass::SetCollisionBBox(const ModelOrganizerStruct& wall)
{
	m_BBox.SetBoundingBox(wall.pos, XMFLOAT3(wall.scale.x, wall.scale.y, 1.0f), wall.rot);
}