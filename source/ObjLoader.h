#pragma once


#include "IModel.h"


class ObjLoader : public IModel
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ObjLoader();
	ObjLoader(const ObjLoader&);
	~ObjLoader();

private:
	bool LoadModel();
	void ReleaseModel();
	void FillBuffers(VertexType outputBuffer[], unsigned long outputIndexes[]);

	bool LoadModel(char* filename);

private:
	ModelType* m_model;
};