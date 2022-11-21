#include "EnemyModelClass.h"


EnemyModelClass::EnemyModelClass()
{
	m_model = nullptr;
	m_CollModelPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


EnemyModelClass::~EnemyModelClass()
{

}


void EnemyModelClass::AddCollisionBBox(const XMFLOAT3& position, const XMFLOAT3& extends)
{
	BBoxCollisionClass bbox(position, extends);
	m_BBoxList.push_back(bbox);
}

void EnemyModelClass::ChangeBBoxPosition(size_t index, const XMFLOAT3& position)
{
	m_BBoxList.at(index).SetPosition(position);
}

// If we're gonna put some more complex collision models that have components relative to main collision model position, put updating here
void EnemyModelClass::ChangeCollisionModelPos(const XMFLOAT3& position)
{
	for (int i = 0; i < m_BBoxList.size(); i++)
	{
		ChangeBBoxPosition(i, position);
	}
	m_CollModelPos = position;
}

bool EnemyModelClass::CheckIntersection(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt, float& dist)
{
	bool ret = false;
	for (vector<BBoxCollisionClass>::iterator it = m_BBoxList.begin(); it != m_BBoxList.end(); it++)
	{
		if (it->Intersects(cameraPosition, cameraLookAt, dist))
		{
			ret = true;
			break;
		}
	}

	return ret;
}

bool EnemyModelClass::CheckIntersection(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt)
{
	bool ret = false;
	for (vector<BBoxCollisionClass>::iterator it = m_BBoxList.begin(); it != m_BBoxList.end(); it++)
	{
		if (it->Intersects(cameraPosition, cameraLookAt))
		{
			ret = true;
			break;
		}
	}

	return ret;
}