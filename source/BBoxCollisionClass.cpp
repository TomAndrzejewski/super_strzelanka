#include "BBoxCollisionClass.h"
#include <fstream>


using DirectX::XMVector3Normalize;
using DirectX::XMVectorSubtract;
using std::ifstream;

BBoxCollisionClass::BBoxCollisionClass()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_extends = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_boundingBox = BoundingOrientedBox(m_position, m_extends, m_orientation);
}

BBoxCollisionClass::BBoxCollisionClass(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation)
{
	m_position = position;
	m_extends = extends;

	XMVECTOR quaternionsVect = XMQuaternionRotationRollPitchYaw(orientation.x, orientation.y, orientation.z);
	XMStoreFloat4(&m_orientation, quaternionsVect);

	m_boundingBox = BoundingOrientedBox(m_position, m_extends, m_orientation);
}

BBoxCollisionClass::~BBoxCollisionClass()
{

}

void BBoxCollisionClass::SetPosition(const XMFLOAT3& position)
{
	m_position = position;
	m_boundingBox = BoundingOrientedBox(m_position, m_extends, m_orientation);
}

void BBoxCollisionClass::SetExtends(const XMFLOAT3& extends)
{
	m_extends = extends;
	m_boundingBox = BoundingOrientedBox(m_position, m_extends, m_orientation);
}

void BBoxCollisionClass::SetOrientation(const XMFLOAT3& orientation)
{
	XMVECTOR quaternionsVect = XMQuaternionRotationRollPitchYaw(orientation.x, orientation.y, orientation.z);
	XMStoreFloat4(&m_orientation, quaternionsVect);

	m_boundingBox = BoundingOrientedBox(m_position, m_extends, m_orientation);
}

void BBoxCollisionClass::SetBoundingBox(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation)
{
	m_position = position;
	m_extends = extends;

	XMVECTOR quaternionsVect = XMQuaternionRotationRollPitchYaw(orientation.x / 57.3, orientation.y / 57.3, orientation.z / 57.3); // angle/57.3 converts from degrees to radians
	XMStoreFloat4(&m_orientation, quaternionsVect);

	m_boundingBox = BoundingOrientedBox(m_position, m_extends, m_orientation);
}

bool BBoxCollisionClass::Intersects(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt, float& dist)
{
	XMVECTOR cameraDirectionVect = XMVectorSubtract(cameraLookAt, cameraPosition);
	cameraDirectionVect = XMVector3Normalize(cameraDirectionVect);

	bool ret = m_boundingBox.Intersects(cameraPosition, cameraDirectionVect, dist);
	return ret;
}

bool BBoxCollisionClass::Intersects(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt)
{
	XMVECTOR cameraDirectionVect = XMVectorSubtract(cameraLookAt, cameraPosition);
	cameraDirectionVect = XMVector3Normalize(cameraDirectionVect);

	float dist; //unused
	bool ret = m_boundingBox.Intersects(cameraPosition, cameraDirectionVect, dist);
	return ret;
}

bool BBoxCollisionClass::Intersects(const BoundingOrientedBox& bbox)
{
	bool ret = m_boundingBox.Intersects(bbox);
	return ret;
}

bool BBoxCollisionClass::ReadBBoxFromFile(const char* bboxFilename, XMFLOAT3& extends)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.  If it could not open the file then exit.
	fin.open(bboxFilename);
	if (fin.fail())
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the extends data.
	fin >> extends.x >> extends.y >> extends.z;

	// Close the model file.
	fin.close();

	return true;
}