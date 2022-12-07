#pragma once

#include "BBoxCollisionClass.h"
#include "WallClass.h"


class PlayerCollisionClass
{
public:
	PlayerCollisionClass();
	PlayerCollisionClass(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation);
	~PlayerCollisionClass();

	void SetCollisionBBox(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation);

	bool CheckIntersection(const BoundingOrientedBox& bbox);
	bool CheckIntersection(WallClass& wall);

private:
	BBoxCollisionClass m_BBox;
};