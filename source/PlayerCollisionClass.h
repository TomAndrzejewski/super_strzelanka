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

	// For collision with ordinary bbox
	bool CheckIntersection(const BoundingOrientedBox& bbox);

	// For collision with walls
	bool CheckIntersection(WallClass& wall);

	// For ray collision with ordinary bbox
	bool CheckIntersection(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt, BBoxCollisionClass& bbox, float& dist);

private:
	BBoxCollisionClass m_BBox;
};