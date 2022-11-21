////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <SimpleMath.h>
using namespace DirectX;
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "IModel.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: CubeModelClass
////////////////////////////////////////////////////////////////////////////////
class CubeModelClass : public IModel
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	CubeModelClass();
	CubeModelClass(const CubeModelClass&);
	~CubeModelClass();

private:
	bool LoadModel();
	void ReleaseModel();
	void FillBuffers(VertexType outputBuffer[], unsigned long outputIndexes[]);

	bool LoadModel(char* filename);

private:
	ModelType* m_model;
};

#endif