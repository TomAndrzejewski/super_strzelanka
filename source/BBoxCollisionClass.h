#pragma once
#include <SimpleMath.h>
#include <directxmath.h>
#include <vector>

using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;
using DirectX::XMVECTOR;
using DirectX::BoundingBox;
using DirectX::BoundingOrientedBox;
using DirectX::XMQuaternionRotationRollPitchYaw;
using DirectX::XMStoreFloat4;


class BBoxCollisionClass
{
public:
	BBoxCollisionClass();
	BBoxCollisionClass(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation);
	~BBoxCollisionClass();

	void SetPosition(const XMFLOAT3& position);
	void SetExtends(const XMFLOAT3& extends);
	void SetOrientation(const XMFLOAT3& orientation);
	void SetBoundingBox(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation);

	bool Intersects(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt, float& dist);
	bool Intersects(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt);

	bool Intersects(const BoundingOrientedBox& bbox);

	BoundingOrientedBox& GetBoundingOrientedBox() { return m_boundingBox; }

	static bool ReadBBoxFromFile(const char* bboxFilename, XMFLOAT3& extends);

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_extends;
	XMFLOAT4 m_orientation;
	BoundingOrientedBox m_boundingBox;
};

