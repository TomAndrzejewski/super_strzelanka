#pragma once

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

class IModel
{
public:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	IModel();
	IModel(const IModel&);
	~IModel();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext* deviceContext, WCHAR*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

protected:
	char* m_ModelFilename;
	int m_vertexCount, m_indexCount;

protected:
	virtual void FillBuffers(VertexType outputBuffer[], unsigned long outputIndexes[]) = 0;

	virtual bool LoadModel() = 0;
	virtual void ReleaseModel() = 0;

private:
	bool InitializeBuffers(ID3D11Device*);

	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext* deviceContext, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	TextureClass* m_Texture;
};

