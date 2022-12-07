#pragma once

#include "BBoxCollisionClass.h"
#include "SimpleModelClass.h"
#include <vector>

using std::vector;
using std::iterator;


// Micro introduction on how to use this class:
// set previously created model via SetModel()
// set all collision boxes you want to use
// set position every new rendering
class EnemyModelClass
{
public:
	EnemyModelClass();
	~EnemyModelClass();

	void AddCollisionBBox(const XMFLOAT3& position, const XMFLOAT3& extends, const XMFLOAT3& orientation);
	void ChangeBBoxPosition(size_t index, const XMFLOAT3& position);

	void ChangeCollisionModelPos(const XMFLOAT3& position);

	bool CheckIntersection(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt, float& dist);
	bool CheckIntersection(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt);


	// Getters and setters
	BBoxCollisionClass& GetBBox(size_t index) { return m_BBoxList.at(index); }
	vector<BBoxCollisionClass>* GetBBoxList() { return &m_BBoxList; }

	SimpleModelClass* GetModel() { return m_model; }
	bool SetModel(SimpleModelClass* model) { if (model == nullptr) return false;	else { m_model = model; return true; } }

private:
	vector<BBoxCollisionClass> m_BBoxList;
	SimpleModelClass* m_model;
	XMFLOAT3 m_CollModelPos;
};

