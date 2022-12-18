#pragma once

#include "..\source\IModel.h"

class ObjModelClass : public IModel
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};


public:
	ObjModelClass();
	ObjModelClass(const ObjModelClass&);
	~ObjModelClass();

private:
	bool LoadModel();
	void ReleaseModel();
	void FillBuffers(VertexType outputBuffer[], unsigned long outputIndexes[]);

	bool LoadModel(char* filename);

private:
	ModelType* m_model;

};
