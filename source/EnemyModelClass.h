#pragma once

#include "BBoxCollisionClass.h"
#include "modelclass.h"
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

	bool Initialize(ModelClass* model);

	void AddCollisionBBox(const XMFLOAT3& position, const XMFLOAT3& extends);
	void ChangeBBoxPosition(size_t index, const XMFLOAT3& position);

	void ChangeCollisionModelPos(const XMFLOAT3& position);

	bool CheckIntersection(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt, float& dist);
	bool CheckIntersection(const XMVECTOR& cameraPosition, const XMVECTOR& cameraLookAt);


	// Getters and setters
	BBoxCollisionClass& GetBBox(size_t index) { return m_BBoxList.at(index); }
	vector<BBoxCollisionClass>* GetBBoxList() { return &m_BBoxList; }

	ModelClass* GetModel() { return m_model; }
	bool SetModel(ModelClass* model) { if (model == nullptr) return false;	else { m_model = model; return true; } }

private:
	vector<BBoxCollisionClass> m_BBoxList;
	ModelClass* m_model;
	XMFLOAT3 m_CollModelPos;
};

