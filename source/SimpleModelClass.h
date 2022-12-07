#pragma once

#include "IModel.h"


class SimpleModelClass : public IModel
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	SimpleModelClass();
	SimpleModelClass(const SimpleModelClass&);
	~SimpleModelClass();

private:
	bool LoadModel();
	void ReleaseModel();
	void FillBuffers(VertexType outputBuffer[], unsigned long outputIndexes[]);

	bool LoadModel(char* filename);

private:
	ModelType* m_model;
};