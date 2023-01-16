////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "CommonConsts.h"
#include "CommonTypes.h"
#include <SimpleMath.h>
#include "BBoxCollisionClass.h"
#include "DebugDraw.h"
#include "inputclass.h"
#include <chrono>

#include <random>
#include <iomanip>


using std::setprecision;
using namespace DirectX::SimpleMath;


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Text = 0;
	m_EnemyModel = 0;
	m_WallModel = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_ModelList = 0;
	m_Frustum = 0;
	m_MenuBitmap = 0;
	m_AimBitmap = 0;
	m_TextureShader = 0;
	m_GunModels[0] = 0;
	m_GunModels[1] = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{

	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	bool result;
	HRESULT hr;
	XMMATRIX baseViewMatrix;

		
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(startPos.x, startPos.y, startPos.z);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set init position of player
	m_Player.RememberLastPosAndRot(startPos, startRot, false);

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the WallModel object.
	m_WallModel = new MultiTextureModelClass;
	if (!m_WallModel)
	{
		return false;
	}

	// Initialize the WallModel object.
	result = m_WallModel->Initialize(m_D3D->GetDevice(), WALL_MODEL_FILENAME, (WCHAR*)WALL_TEXTURE_FILENAME, (WCHAR*)BRICK_TEXTURE_FILENAME);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	// Initialize WallList
	for (int i = 0; i < sizeOf_walls; i++)
	{
		WallClass Wall;

		// Pass pointer to a model, so only one model needs to be initialized for i.e. 10 walls instances
		Wall.SetModel(m_WallModel);

		// Set initial position
		Wall.SetPosAndRotAndScale(walls[i]);

		// Add needed collisionBBox
		Wall.SetCollisionBBox(walls[i]);

		m_WallList.push_back(Wall);
	}

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<> distr(0.3f, 1.0f);
	for (int i = 0; i < sizeOf_walls; i++)
	{
		if (i < sizeOf_walls - 2) //sciany
		{
			m_WallColors.push_back(XMFLOAT4(distr(eng), distr(eng), distr(eng), 1.0f));
		}
		else //podloga i sufit
		{
			m_WallColors.push_back(XMFLOAT4(0.7f, 0.5f, 0.5f, 1.0f));
		}
		
		//m_WallColors.push_back(XMFLOAT4(0.9f, 0.5f, 0.5f, 1.0f));
	}


	// Create the text object.
	m_EnemyModel = new MultiTextureModelClass;
	if (!m_EnemyModel)
	{
		return false;
	}

	// Initialize the model object.
	result = m_EnemyModel->Initialize(m_D3D->GetDevice(), CUBE_MODEL_FILENAME, (WCHAR*)CUBE_TEXTURE_FILENAME, (WCHAR*)CUBE_TEXTURE_FILENAME);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_GunModels[0] = new MultiTextureModelClass;
	if (!m_GunModels[0])
	{
		return false;
	}

	// Initialize the gun model object.
	result = m_GunModels[0]->Initialize(m_D3D->GetDevice(), GUN1_MODEL_FILENAME, (WCHAR*)GUN1_TEXTURE_FILENAME, (WCHAR*)GUN1_TEXTURE_FILENAME);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_GunModels[1] = new MultiTextureModelClass;
	if (!m_GunModels[1])
	{
		return false;
	}

	// Initialize the gun model object.
	result = m_GunModels[1]->Initialize(m_D3D->GetDevice(), GUN2_MODEL_FILENAME, (WCHAR*)GUN2_TEXTURE_FILENAME, (WCHAR*)GUN2_TEXTURE_FILENAME);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize list of simple cube enemies
	XMFLOAT3 position(0.0f, 0.0f, 0.0f);
	XMFLOAT3 extends[SIMPLE_CUBE_BBOX_NUMBER];
	XMFLOAT3 orientation[SIMPLE_CUBE_BBOX_NUMBER];
	for (int i = 0; i < SIMPLE_CUBE_BBOX_NUMBER; i++)
	{
		// Read bbox needed data
		if (BBoxCollisionClass::ReadBBoxFromFile(simpleCubeBBoxFiles[i].c_str(), extends[i]) == false) { return false; }
		orientation[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < sizeOf_enemies; i++)
	{
		EnemyClass Enemy;
		
		// Pass pointer to a model, so only one model needs to be initialized for i.e. 10 enemy instances
		Enemy.SetModel(m_EnemyModel);

		// Add all needed collisionBBoxes
		for (int j = 0; j < SIMPLE_CUBE_BBOX_NUMBER; j++)
		{
			Enemy.AddCollisionBBox(enemies[i].pos, enemies[i].scale, enemies[i].rot);
		}

		// Set initial position
		Enemy.SetPosAndRotAndScale(enemies[i]);

		//TODOTOMKA: potem to wywal
		Enemy.PlayerStarted();

		m_EnemyList.push_back(Enemy);
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_TextureShader = new TextureShaderClass();
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, 0.5f, 0.0f);

	// Create the model list object.
	m_ModelList = new ModelListClass;
	if(!m_ModelList)
	{
		return false;
	}

	// Initialize the model list object.
	result = m_ModelList->Initialize(25);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if(!m_Frustum)
	{
		return false;
	}

	// Ending position BBox initialization
	m_EndingAreaBBox.SetBoundingBox(XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f - 1 / 3.34f - 1 / (mainSquareLen / 5)) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 / 3.0f - 1 / 3.0f) * mainSquareLen - mainSquareLen / 8), XMFLOAT3(mainSquareLen / 5.8f, wallsHeight, mainSquareLen / 8), XMFLOAT3(0.0f, 0.0f, 0.0f));

	// For DebugDraw
	m_states = std::make_unique<CommonStates>(m_D3D->GetDevice());

	m_effect = std::make_unique<BasicEffect>(m_D3D->GetDevice());
	m_effect->SetVertexColorEnabled(true);

	hr = CreateInputLayoutFromEffect<VertexType>(m_D3D->GetDevice(), m_effect.get(),
		m_inputLayout.ReleaseAndGetAddressOf());
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}
	m_batch = std::make_unique<PrimitiveBatch<VertexType>>(m_D3D->GetDeviceContext());

	// Create the bitmap object.
	m_MenuBitmap = new BitmapClass;
	if (!m_MenuBitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_MenuBitmap->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)MENU_TEXTURE_FILENAME, 1920, 1080);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the menu bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_AimBitmap = new BitmapClass;
	if (!m_AimBitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_AimBitmap->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)AIM_TEXTURE_FILENAME, 32, 32);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the aim bitmap object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the bitmap object.
	if (m_MenuBitmap)
	{
		m_MenuBitmap->Shutdown();
		delete m_MenuBitmap;
		m_MenuBitmap = 0;
	}

	if (m_AimBitmap)
	{
		m_AimBitmap->Shutdown();
		delete m_AimBitmap;
		m_AimBitmap = 0;
	}

	// Release the frustum object.
	if(m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the model list object.
	if(m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_EnemyModel)
	{
		m_EnemyModel->Shutdown();
		delete m_EnemyModel;
		m_EnemyModel = 0;
	}

	// Release the model object.
	if (m_WallModel)
	{
		m_WallModel->Shutdown();
		delete m_WallModel;
		m_WallModel = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	m_states.release();
	m_effect.release();
	m_batch.release();
	m_inputLayout.Reset();

	return;
}


bool GraphicsClass::Frame(float rotationY, float rotationX, float positionZ, float positionX)
{
	// Set the position of the camera.
	m_Camera->SetPosition(positionX, 0.0f, positionZ);

	// Set the rotation of the camera.
	m_Camera->SetRotation(rotationX, rotationY, 0.0f);

	return true;
}

bool GraphicsClass::Render(PositionClass* positionClass, InputClass* inputClass)
{
	static auto start = std::chrono::high_resolution_clock::now();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	XMFLOAT4 color;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Process player collision if on
	viewMatrix = (playerCollisionON) ? ProcessPlayerCollision(positionClass) : m_Camera->GetViewMatrix();

	if (inputClass->IsLPMPressed())
	{
		auto finish = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> elapsed = finish - start;

		if (elapsed.count() >= shootingDelay)
		{
			char str[500];
			sprintf_s(str, "DELAY STRZALU, %f\n", elapsed.count());
			OutputDebugStringA(str);

			start = std::chrono::high_resolution_clock::now();
			ProcessShootingCollision();
		}
	}

	RenderWalls(m_WallList, m_WallColors, viewMatrix);

	RenderEnemies(m_EnemyList, viewMatrix);

	if (drawCollisionBBoxes)
	{
		RenderCollisionBoxes();
	}

	RenderGun(viewMatrix, orthoMatrix, m_ScreenWidth, m_ScreenHeight);

	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderMenu(int activeOption)
{
	XMMATRIX orthoMatrix, viewMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();
	viewMatrix = m_Camera->GetViewMatrix();

	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_D3D->TurnOnAlphaBlending();
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_MenuBitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MenuBitmap->GetIndexCount(), m_D3D->GetWorldMatrix(), viewMatrix, orthoMatrix, m_MenuBitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	m_Text->SetActiveMenuOption(activeOption);

	result = RenderText();
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();


	return true;
}

bool GraphicsClass::RenderText()
{
	bool result;

	result = m_Text->Render(m_D3D->GetDeviceContext(), m_D3D->GetWorldMatrix(), m_D3D->GetOrthoMatrix());
	if (!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::ProcessShootingCollision()
{
	int minKilledEnemyDistIndex = 0;
	float minKilledEnemyDist = FLT_MAX;
	float minWallHitDist = FLT_MAX;
	float minObstacleHitDist = FLT_MAX;
	float dist;

	// Go through all the walls
	for (int index = 0; index < sizeOf_walls; index++)
	{
		WallClass* pWall = &m_WallList.at(index);

		if (m_Player.CheckIntersection(m_Camera->GetPositionVector(), m_Camera->GetLookAtVector(), pWall->GetBBox(), dist))
		{
			minWallHitDist = (dist < minWallHitDist) ? dist : minWallHitDist;
		}
	}

	// Go through all the enemies
	for (int index = 0; index < sizeOf_enemies; index++)
	{
		EnemyClass* pEnemy = &m_EnemyList.at(index);

		if (pEnemy->GetEnemyState() == PlayerShootEnemyEnum)
		{
			continue;
		}

		if (pEnemy->CheckIfEnemyIsHit(m_Camera->GetPositionVector(), m_Camera->GetLookAtVector(), dist))
		{
			minKilledEnemyDist = (dist < minKilledEnemyDist) ? dist : minKilledEnemyDist;
			minKilledEnemyDistIndex = index;
		}
	}

	// Go through all the obstacles


	if ((minKilledEnemyDist > minWallHitDist) || (minKilledEnemyDist > minObstacleHitDist) || (minKilledEnemyDist == FLT_MAX))
	{
		// Nothing to do, first hit wall or obstacle or no enemy was hit at all so enemy was not hit
		return;
	}

	// Enemy got hit, set his status as being hit and inform player that there has been a kill
	m_EnemyList.at(minKilledEnemyDistIndex).EnemyGotHit();
	m_Player.PlayerKilledEnemy();
}

XMMATRIX& GraphicsClass::ProcessPlayerCollision(PositionClass* positionClass)
{
	// Check for player collision with walls
	XMFLOAT3 pos, rot;
	XMStoreFloat3(&pos, m_Camera->GetPositionVector());
	XMStoreFloat3(&rot, m_Camera->GetRotationVector());
	m_Player.SetCollisionBBox(pos, XMFLOAT3(1.0f, 1.0f, 1.0f), rot);

	bool collisionOccured = false;
	for (int i = 0; i < sizeOf_walls; i++)
	{
		if (m_Player.CheckIntersection(m_WallList.at(i)))
		{
			collisionOccured = true;
			break;
		}
	}
	XMStoreFloat3(&pos, m_Camera->GetPositionVector());
	XMStoreFloat3(&rot, m_Camera->GetRotationVector());
	m_Player.RememberLastPosAndRot(pos, rot, collisionOccured);
	m_Player.RememberLastPosAndRot(pos, rot, false);
	m_Player.GetLastPosAndRot(pos, rot);

	// Set camera pos and rot after checking for collision with walls
	m_Camera->SetPosition(pos.x, pos.y, pos.z);
	m_Camera->SetRotation(rot.x, rot.y, rot.z);
	positionClass->SetPosition(pos.x, pos.z);
	positionClass->SetRotation(rot.x, rot.y);
	m_Camera->Render();


	// Check if player get to ending position
	if (m_Player.CheckIntersection(m_EndingAreaBBox.GetBoundingOrientedBox()) && m_Player.HowManyEnemiesKilled() >= sizeOf_enemies)
	{
		m_Camera->SetPosition(startPos.x, startPos.y, startPos.z);
		m_Camera->SetRotation(startRot.x, startRot.y, startRot.z);
		positionClass->SetPosition(startPos.x, startPos.z);
		positionClass->SetRotation(startRot.x, startRot.y);
		m_Camera->Render();
	}

	return m_Camera->GetViewMatrix();
}

void GraphicsClass::SetCurrentGun(int index)
{
	m_Gun.SetModel(m_GunModels[index]);
}

void GraphicsClass::RenderEnemies(vector<EnemyClass>& enemies, XMMATRIX& viewMatrix)
{
	// Go through all the enemies
	for (int index = 0; index < sizeOf_enemies; index++)
	{
		XMFLOAT4 color;
		EnemyClass* pEnemy = &enemies.at(index);
		if (pEnemy->GetEnemyState() == PlayerShootEnemyEnum)
		{
			color = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f); // blue
		}
		else
		{
			color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f); // white
		}
		// Move the model to the location it should be rendered at.
		XMMATRIX worldMatrix = pEnemy->GetModelToWorldMatrix();

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		pEnemy->GetModel()->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		m_LightShader->Render(m_D3D->GetDeviceContext(), pEnemy->GetModel()->GetIndexCount(), worldMatrix, viewMatrix, m_D3D->GetProjectionMatrix(),
			pEnemy->GetModel()->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), color);
	}
}

void GraphicsClass::RenderWalls(vector<WallClass>& walls, vector<XMFLOAT4>& colorsOfWalls, XMMATRIX& viewMatrix)
{
	// Rendering of walls
	for (int index = 0; index < sizeOf_walls; index++)
	{
		WallClass* pWall = &walls.at(index);

		XMMATRIX worldMatrix = pWall->GetModelToWorldMatrix();

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		pWall->GetModel()->Render(m_D3D->GetDeviceContext());

		// Get color of wall
		XMFLOAT4 color = colorsOfWalls.at(index);

		// Render the model using the light shader.
		m_LightShader->Render(m_D3D->GetDeviceContext(), pWall->GetModel()->GetIndexCount(), worldMatrix, viewMatrix, m_D3D->GetProjectionMatrix(),
			pWall->GetModel()->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), color);
	}
}

void GraphicsClass::RenderCollisionBoxes()
{
	// Operations needed for debug drawing
	m_D3D->GetDeviceContext()->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_D3D->GetDeviceContext()->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_D3D->GetDeviceContext()->RSSetState(m_states->CullNone());

	m_effect->Apply(m_D3D->GetDeviceContext());

	// View and projection matrices
	m_effect->SetView(m_Camera->GetViewMatrix());
	m_effect->SetProjection(m_D3D->GetProjectionMatrix());

	m_D3D->GetDeviceContext()->IASetInputLayout(m_inputLayout.Get());

	m_batch->Begin();

	// Walls' collision bboxes
	for (int i = 0; i < sizeOf_walls; i++)
	{
		DX::Draw(m_batch.get(), m_WallList.at(i).GetBBox().GetBoundingOrientedBox(), Colors::Blue);
	}

	// Enemies' collision bboxes
	for (int i = 0; i < sizeOf_enemies; i++)
	{
		DX::Draw(m_batch.get(), m_EnemyList.at(i).GetBBox(0).GetBoundingOrientedBox(), Colors::White);
	}

	// Ending area bbox
	DX::Draw(m_batch.get(), m_EndingAreaBBox.GetBoundingOrientedBox(), Colors::White);

	m_batch->End();
}

void GraphicsClass::SetWireFrame(bool on)
{
	if(on)
		m_D3D->GetDeviceContext()->RSSetState(m_states->Wireframe());
	else
		m_D3D->GetDeviceContext()->RSSetState(m_states->CullCounterClockwise());
}

void GraphicsClass::RenderGun(XMMATRIX& viewMatrix, XMMATRIX& orthoMatrix, int screenW, int screenH)
{
	// Get player position and rotation
	XMFLOAT3 playerPos, playerRot, posOffset;
	m_Player.GetLastPosAndRot(playerPos, playerRot);

	m_Gun.SetRelativePositionAndRotation(playerPos, playerRot);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Gun.GetModel()->Render(m_D3D->GetDeviceContext());

	//// Render the model using the light shader.
	m_LightShader->Render(m_D3D->GetDeviceContext(), m_Gun.GetModel()->GetIndexCount(), m_Gun.GetModelToWorldMatrix(), viewMatrix, m_D3D->GetProjectionMatrix(),
		m_Gun.GetModel()->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	

	//render aim
	m_D3D->TurnOnAlphaBlending();
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_AimBitmap->Render(m_D3D->GetDeviceContext(), (screenW - m_AimBitmap->GetBitmapWidth()) / 2, (screenH - m_AimBitmap->GetBitmapHeight()) / 2);


	// Render the bitmap with the texture shader.
	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_AimBitmap->GetIndexCount(), m_Gun.GetModelToWorldMatrix(), viewMatrix, orthoMatrix, m_AimBitmap->GetTexture());

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

}