#include "PlayerCollisionClass.h"

PlayerCollisionClass::PlayerCollisionClass()
{

}

PlayerCollisionClass::PlayerCollisionClass(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation)
{
	SetCollisionBBox(position, extends, orientation);
}

PlayerCollisionClass::~PlayerCollisionClass()
{

}

void PlayerCollisionClass::SetCollisionBBox(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation)
{
	m_BBox.SetBoundingBox(position, extends, orientation);
}

bool PlayerCollisionClass::CheckIntersection(const BoundingOrientedBox& bbox)
{
	return m_BBox.Intersects(bbox);
}

bool PlayerCollisionClass::CheckIntersection(WallClass& wall)
{
	return m_BBox.Intersects(wall.GetBBox().GetBoundingOrientedBox());
}