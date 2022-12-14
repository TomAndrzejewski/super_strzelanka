////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <vector>


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "textclass.h"
#include "SimpleModelClass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "modellistclass.h"
#include "frustumclass.h"
#include "EnemyClass.h"
#include "WallClass.h"
#include "PlayerClass.h"
#include "positionclass.h"

#include <debugapi.h>
#include "BufferHelpers.h"
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"

using VertexType = DirectX::VertexPositionColor;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float, float, float, float);
	bool Render(PositionClass* positionClass);

	void ProcessShootingCollision();
	void RenderEnemies(vector<EnemyClass>& enemies, XMMATRIX& viewMatrix);
	void RenderWalls(vector<WallClass>& walls, vector<XMFLOAT4>& colorsOfWalls, XMMATRIX& viewMatrix);
	void RenderCollisionBoxes();
	XMMATRIX& ProcessPlayerCollision(PositionClass* positionClass);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	TextClass* m_Text;
	SimpleModelClass* m_EnemyModel;
	SimpleModelClass* m_WallModel;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	ModelListClass* m_ModelList;
	FrustumClass* m_Frustum;
	vector<EnemyClass> m_EnemyList;
	vector<WallClass> m_WallList;
	vector<XMFLOAT4> m_WallColors;
	PlayerClass m_Player;
	BBoxCollisionClass m_EndingAreaBBox;

	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

#endif