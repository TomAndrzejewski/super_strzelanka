#pragma once
#include <SimpleMath.h>
#include <directxmath.h>
#include <vector>

using DirectX::XMFLOAT3;
using DirectX::XMVECTOR;
using DirectX::BoundingBox;


class BBoxCollisionClass
{
public:
	BBoxCollisionClass();
	BBoxCollisionClass(const XMFLOAT3& position, const XMFLOAT3& extends);
	~BBoxCollisionClass();

	void SetPosition(const XMFLOAT3& position);
	void SetExtends(const XMFLOAT3& extends);
	void SetBoundingBox(const XMFLOAT3& position, const XMFLOAT3& extends);

	bool Intersects(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt, float& dist);
	bool Intersects(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt);

	static bool ReadBBoxFromFile(const char* bboxFilename, XMFLOAT3& extends);

private:
	// For BoundingBox
	XMFLOAT3 m_position;
	XMFLOAT3 m_extends;
	BoundingBox m_boundingBox;
};

